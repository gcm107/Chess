"""
Encapsulates the worker which trains ChessModels using game data from recorded games from a file.
"""
import os
from collections import deque
from concurrent.futures import ProcessPoolExecutor
from datetime import datetime
from logging import getLogger
from time import sleep
from random import shuffle

import numpy as np

from chess_zero.agent.model_chess import ChessModel
from chess_zero.config import Config
from chess_zero.env.chess_env import canon_input_planes, is_black_turn, testeval
from chess_zero.lib.data_helper import get_game_data_filenames, read_game_data_from_file, get_next_generation_model_dirs
from chess_zero.lib.model_helper import load_best_model_weight

from keras.optimizers import Adam
from keras.callbacks import TensorBoard
logger = getLogger(__name__)


def start(config: Config):
    """
    Helper method which just kicks off the optimization using the specified config
    :param Config config: config to use
    """
    return OptimizeWorker(config).start()


class OptimizeWorker:
    """
    Worker which optimizes a ChessModel by training it on game data

    Attributes:
        :ivar Config config: config for this worker
        :ivar ChessModel model: model to train
        :ivar dequeue,dequeue,dequeue dataset: tuple of dequeues where each dequeue contains game states,
            target policy network values (calculated based on visit stats
                for each state during the game), and target value network values (calculated based on
                    who actually won the game after that state)
        :ivar ProcessPoolExecutor executor: executor for running all of the training processes
    """
    def __init__(self, config: Config):
        self.config = config
        self.model = None  # type: ChessModel
        self.dataset = deque(),deque(),deque()
        self.executor = ProcessPoolExecutor(max_workers=config.trainer.cleaning_processes)

    def start(self):
        """
        Load the next generation model from disk and start doing the training endlessly.
        """
        self.model = self.load_model()
        self.training()

    def training(self):
        """
        Does the actual training of the model, running it on game data. Endless.
        """
        self.compile_model()
        self.filenames = deque(get_game_data_filenames(self.config.resource))
        shuffle(self.filenames)
        total_steps = self.config.trainer.start_total_steps

        while True:
            self.fill_queue()
            steps = self.train_epoch(self.config.trainer.epoch_to_checkpoint)
            total_steps += steps
            self.save_current_model()
            a, b, c = self.dataset
            while len(a) > self.config.trainer.dataset_size/2:
                a.popleft()
                b.popleft()
                c.popleft()

    def train_epoch(self, epochs):
        """
        Runs some number of epochs of training
        :param int epochs: number of epochs
        :return: number of datapoints that were trained on in total
        """
        tc = self.config.trainer
        state_ary, policy_ary, value_ary = self.collect_all_loaded_data()
        tensorboard_cb = TensorBoard(log_dir="./logs", histogram_freq=1)
        self.model.model.fit(state_ary, [policy_ary, value_ary],
                             batch_size=tc.batch_size,
                             epochs=epochs,
                             shuffle=True,
                             validation_split=0.02,
                             callbacks=[tensorboard_cb])
        steps = (state_ary.shape[0] // tc.batch_size) * epochs
        return steps

    def compile_model(self):
        """
        Compiles the model to use optimizer and loss function tuned for supervised learning
        """
        opt = Adam()
        losses = ['categorical_crossentropy', 'mean_squared_error'] # avoid overfit for supervised 
        self.model.model.compile(optimizer=opt, loss=losses, loss_weights=self.config.trainer.loss_weights)

    def save_current_model(self):
        rc = self.config.resource
        model_id = datetime.now().strftime("%Y%m%d-%H%M%S.%f")
        model_dir = os.path.join(rc.next_generation_model_dir, rc.next_generation_model_dirname_tmpl % model_id)
        os.makedirs(model_dir, exist_ok=True)
        config_path = os.path.join(model_dir, rc.next_generation_model_config_filename)
        weight_path = os.path.join(model_dir, rc.next_generation_model_weight_filename)
        
        # Ensure the weight filename ends with .weights.h5
        if not weight_path.endswith('.weights.h5'):
            weight_path = weight_path.rsplit('.', 1)[0] + '.weights.h5'
        
        try:
            self.model.save(config_path, weight_path)
            logger.debug(f"Saved model to {config_path} and {weight_path}")
        except Exception as e:
            logger.error(f"Failed to save model: {str(e)}")
            try:
                self.model.model.save_weights(weight_path)
                logger.debug(f"Saved model weights to {weight_path}")
                with open(config_path, "w") as f:
                    json.dump(self.model.model.get_config(), f)
                logger.debug(f"Saved model config to {config_path}")
            except Exception as e2:
                logger.error(f"Failed to save model using alternative method: {str(e2)}")


    def fill_queue(self):
        """
        Fills the self.dataset queues with data from the training dataset.
        """
        futures = deque()
        with ProcessPoolExecutor(max_workers=self.config.trainer.cleaning_processes) as executor:
            for _ in range(self.config.trainer.cleaning_processes):
                if len(self.filenames) == 0:
                    break
                filename = self.filenames.popleft()
                logger.debug(f"loading data from {filename}")
                futures.append(executor.submit(load_data_from_file,filename))
            while futures and len(self.dataset[0]) < self.config.trainer.dataset_size:
                for x,y in zip(self.dataset,futures.popleft().result()):
                    x.extend(y)
                if len(self.filenames) > 0:
                    filename = self.filenames.popleft()
                    logger.debug(f"loading data from {filename}")
                    futures.append(executor.submit(load_data_from_file,filename))

    def collect_all_loaded_data(self):
        """

        :return: a tuple containing the data in self.dataset, split into
        (state, policy, and value).
        """
        state_ary,policy_ary,value_ary=self.dataset

        state_ary1 = np.asarray(state_ary, dtype=np.float32)
        policy_ary1 = np.asarray(policy_ary, dtype=np.float32)
        value_ary1 = np.asarray(value_ary, dtype=np.float32)
        return state_ary1, policy_ary1, value_ary1

    def load_model(self):
        model = ChessModel(self.config)
        dirs = get_next_generation_model_dirs(self.config.resource)
        if not dirs:
            logger.debug("No next generation models found, loading best model")
            if not load_best_model_weight(model):
                logger.warning("Failed to load best model weight, initializing with random weights")
                model.build()
        else:
            latest_dir = dirs[-1]
            config_path = os.path.join(latest_dir, self.config.resource.next_generation_model_config_filename)
            weight_path = os.path.join(latest_dir, self.config.resource.next_generation_model_weight_filename)
            if not os.path.exists(config_path) or not os.path.exists(weight_path):
                logger.warning(f"Config or weight file not found in {latest_dir}, loading best model")
                if not load_best_model_weight(model):
                    logger.warning("Failed to load best model weight, initializing with random weights")
                    model.build()
            else:
                model.load(config_path, weight_path)
        return model


def load_data_from_file(filename):
    data = read_game_data_from_file(filename)
    return convert_to_cheating_data(data)


def convert_to_cheating_data(data):
    """
    :param data: format is SelfPlayWorker.buffer
    :return:
    """
    state_list = []
    policy_list = []
    value_list = []
    for state_fen, policy, value in data:

        state_planes = canon_input_planes(state_fen)

        if is_black_turn(state_fen):
            policy = Config.flip_policy(policy)

        move_number = int(state_fen.split(' ')[5])
        value_certainty = min(5, move_number)/5 # reduces the noise of the opening... plz train faster
        sl_value = value*value_certainty + testeval(state_fen, False)*(1-value_certainty)

        state_list.append(state_planes)
        policy_list.append(policy)
        value_list.append(sl_value)

    return np.asarray(state_list, dtype=np.float32), np.asarray(policy_list, dtype=np.float32), np.asarray(value_list, dtype=np.float32)

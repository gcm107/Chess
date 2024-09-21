from alpha_net import ChessNet
import torch
import os

net = ChessNet()
torch.save({'state_dict': net.state_dict()}, os.path.join("./model_data/", "current_net_trained8_iter1.pth.tar"))
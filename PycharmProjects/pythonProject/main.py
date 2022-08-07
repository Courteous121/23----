import os
import torch
import torchvision
from torch.autograd import Variable
from torchvision import datasets, transforms
from torch.utils.data import DataLoader
import matplotlib.pyplot as plt
import numpy as np
import cv2


from torch.utils.data import Dataset
from torch.utils.data import Sampler

def eg_1_2_0():
  """
  Eg1.2.0 : MNIST
  """
  from torchvision.datasets.mnist import MNIST
  train_dataset = MNIST(root="./mnist_data",
                        train=True,
                        transform=None,
                        download=True)

  print("type(train_dataset): {}".format(type(train_dataset)))  # <class 'torchvision.datasets.mnist.MNIST'>
  index = 0
  print("train_dataset[{}]: {}".format(index, train_dataset[index]))  # (PIL.Image.Image, 5)
  import matplotlib.pyplot as plt
  plt.imshow(train_dataset[index][0], cmap ='gray')
  print("len(train_dataset): {}".format(len(train_dataset)))


def eg_1_2_1():
  """
  Eg1.2.1 : transforms
  """
  from torchvision.datasets.mnist import MNIST
  from torchvision import transforms

  transform = transforms.Compose(
    [
      transforms.ToTensor(),
      transforms.Normalize(mean=(0.5,), std=(0.5,))
    ]
  )
  train_dataset = MNIST(root="./mnist_data",
                        train=True,
                        transform=transform,
                        target_transform=None,
                        download=False)

  index = 0
  print("type(train_dataset[{}]): {}".format(index, type(train_dataset[index])))  # <class 'tuple'>
  print("type(train_dataset[{}][0]): {}".format(index, type(train_dataset[index][0])))  # <class 'torch.Tensor'>
  print("train_dataset[{}][0].shape: {}".format(index, train_dataset[index][0].shape))  # torch.Size([1, 28, 28])
  print("type(train_dataset[{}][1]): {}".format(index, type(train_dataset[index][1])))  # <class 'int'>

import numpy as np

import torch
import torch.nn as nn
import torch.optim as optim
import torchvision.datasets as dset
from torch.utils.data.sampler import SubsetRandomSampler

from torchvision import transforms

class PyTorchHelper:

    def __init__(self,  batch_size, data):
        self.batch_size = batch_size
        self.data = data
        self.device = torch.device("cuda:0")  # Let's make sure GPU is available!

    def split(self, validation_split):

        data_size = self.data.data.shape[0]
        split = int(np.floor(validation_split * data_size))
        indices = list(range(data_size))
        np.random.shuffle(indices)

        train_indices, val_indices = indices[split:], indices[:split]

        return train_indices, val_indices

    def train_model(self, model, train_loader, val_loader, loss, optimizer, num_epochs, scheduler=None):
        loss_history = []
        train_history = []
        val_history = []
        for epoch in range(num_epochs):
            model.train()  # Enter train mode

            loss_accum = 0
            correct_samples = 0
            total_samples = 0
            for i_step, (x, y) in enumerate(train_loader):
                x_gpu = x.to(self.device)
                y_gpu = y.to(self.device)
                prediction = model(x_gpu)
                loss_value = loss(prediction, y_gpu)
                optimizer.zero_grad()
                loss_value.backward()
                optimizer.step()

                _, indices = torch.max(prediction, 1)
                correct_samples += torch.sum(indices == y_gpu)
                total_samples += y.shape[0]

                loss_accum += loss_value

            if scheduler is not None:
                scheduler.step()

            ave_loss = loss_accum / i_step
            train_accuracy = float(correct_samples) / total_samples
            val_accuracy = self.compute_accuracy(model, val_loader)

            loss_history.append(float(ave_loss))
            train_history.append(train_accuracy)
            val_history.append(val_accuracy)

            print("Average loss: %f, Train accuracy: %f, Val accuracy: %f" % (ave_loss, train_accuracy, val_accuracy))

        return loss_history, train_history, val_history

    def compute_accuracy(self, model, loader):
        """
        Computes accuracy on the dataset wrapped in a loader

        Returns: accuracy as a float value between 0 and 1
        """
        model.eval()  # Evaluation mode
        # TODO: Copy implementation from previous assignment
        # Don't forget to move the data to device before running it through the model!

        total_samples = 0
        correct_samples = 0
        for i_step, (x, y) in enumerate(loader):
            x_gpu = x.to(self.device)
            y_gpu = y.to(self.device)

            prediction = model(x_gpu)

            _, indices = torch.max(prediction, 1)

            total_samples += y.shape[0]
            correct_samples += torch.sum(indices == y_gpu)

        return float(correct_samples) / total_samples
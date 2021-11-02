import numpy as np

from layers import FullyConnectedLayer, ReLULayer, softmax_with_cross_entropy, l2_regularization


class TwoLayerNet:
    """ Neural network with two fully connected layers """

    def __init__(self, n_input, n_output, hidden_layer_size, reg):
        """
        Initializes the neural network

        Arguments:
        n_input, int - dimension of the model input
        n_output, int - number of classes to predict
        hidden_layer_size, int - number of neurons in the hidden layer
        reg, float - L2 regularization strength
        """
        # TODO Create necessary layers

        self.layers = []
        self.layers.append(FullyConnectedLayer(n_input, hidden_layer_size))
        self.layers.append(ReLULayer())
        self.layers.append(FullyConnectedLayer(hidden_layer_size, n_output))
        self.reg = reg

    def compute_loss_and_gradients(self, X, y):
        """
        Computes total loss and updates parameter gradients
        on a batch of training examples

        Arguments:
        X, np array (batch_size, input_features) - input data
        y, np array of int (batch_size) - classes
        """
        # Before running forward and backward pass through the model,
        # clear parameter gradients aggregated from the previous pass
        # TODO Set parameter gradient to zeros
        # Hint: using self.params() might be useful!

        for param in self.params().values():
            param.grad = 0

        # TODO Compute loss and fill param gradients
        # by running forward and backward passes through the model
        for layer in self.layers:
            X = layer.forward(X)
        loss, grad = softmax_with_cross_entropy(X, y)

        for layer in reversed(self.layers):
            grad = layer.backward(grad)
        
        # After that, implement l2 regularization on all params
        # Hint: self.params() is useful again!

        for param in self.params().values():
            loss_l2, grad_l2 = l2_regularization(param.value, self.reg)
            loss += loss_l2
            param.grad += grad_l2

        return loss

    def predict(self, X):
        """
        Produces classifier predictions on the set

        Arguments:
          X, np array (test_samples, num_features)

        Returns:
          y_pred, np.array of int (test_samples)
        """
        # TODO: Implement predict
        # Hint: some of the code of the compute_loss_and_gradients
        # can be reused

        for layer in self.layers:
            X = layer.forward(X)
        pred = np.argmax(X, axis=1)
        return pred

    def params(self):
        result = {}

        # TODO Implement aggregating all of the params

        result['W1'] = self.layers[0].W
        result['B1'] = self.layers[0].B

        result['W2'] = self.layers[2].W
        result['B2'] = self.layers[2].B

        return result

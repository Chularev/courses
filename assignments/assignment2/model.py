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

        self.FCL1 = FullyConnectedLayer(n_input, hidden_layer_size)
        self.ReLu = ReLULayer()
        self.FCL2 = FullyConnectedLayer(hidden_layer_size, n_output)
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

        params = self.params()
        params['W1'] = params['B1'] = params['W2'] = params['B2'] = 0
        
        # TODO Compute loss and fill param gradients
        # by running forward and backward passes through the model
        l1_result = self.FCL1.forward(X)
        relu_result = self.ReLu.forward(l1_result)
        l2_result = self.FCL2.forward(relu_result)

        loss, grad = softmax_with_cross_entropy(l2_result,y)

        grad = self.FCL2.backward(grad)
        grad = self.ReLu.backward(grad)
        grad = self.FCL1.backward(grad)
        
        # After that, implement l2 regularization on all params
        # Hint: self.params() is useful again!

        W1_loss, W1_grad = l2_regularization(self.FCL1.W.value, self.reg)
        self.FCL1.W.grad += W1_grad
        B1_loss, B1_grad = l2_regularization(self.FCL1.B.value, self.reg)
        self.FCL1.B.grad += B1_grad
        W2_loss, W2_grad = l2_regularization(self.FCL2.W.value, self.reg)
        self.FCL2.W.grad += W2_grad
        B2_loss, B2_grad = l2_regularization(self.FCL2.B.value, self.reg)
        self.FCL2.B.grad += B2_grad
        self.params()

        loss = loss + W1_loss + B1_loss + W2_loss + B2_loss

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
        pred = np.zeros(X.shape[0], np.int)

        raise Exception("Not implemented!")
        return pred

    def params(self):
        result = {}

        # TODO Implement aggregating all of the params

        result['W1'] = self.FCL1.W
        result['B1'] = self.FCL1.B

        result['W2'] = self.FCL2.W
        result['B2'] = self.FCL2.B

        return result

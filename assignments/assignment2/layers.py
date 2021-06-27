import numpy as np


def l2_regularization(W, reg_strength):
    """
    Computes L2 regularization loss on weights and its gradient

    Arguments:
      W, np array - weights
      reg_strength - float value

    Returns:
      loss, single value - l2 regularization loss
      gradient, np.array same shape as W - gradient of weight by l2 loss
    """
    # TODO: Copy from the previous assignment
    loss = reg_strength * np.sum(np.power(W, 2))
    grad = reg_strength * 2 * W
    return loss, grad

def softmax(predictions):
    '''
    Computes probabilities from scores

    Arguments:
      predictions, np array, shape is either (N) or (batch_size, N) -
        classifier output

    Returns:
      probs, np array of the same shape as predictions -
        probability for every class, 0..1
    '''

    if len(predictions.shape) == 2:
        s = np.max(predictions, axis=1)
        e_x = np.exp(predictions - s[:, np.newaxis])
        div = np.sum(e_x, axis=1)
        return e_x / div[:, np.newaxis]

    exps = np.exp(predictions - np.max(predictions))
    return exps / np.sum(exps)

def cross_entropy_loss(probs, target_index):
    '''
    Computes cross-entropy loss

    Arguments:
      probs, np array, shape is either (N) or (batch_size, N) -
        probabilities for every class
      target_index: np array of int, shape is (1) or (batch_size) -
        index of the true class for given sample(s)

    Returns:
      loss: single value
    '''
    # TODO implement cross-entropy
    # Your final implementation shouldn't have any loops
    if type(target_index) == int:
        target_index = np.asarray( [target_index])

    if len(target_index.shape) == 1:
        probs = probs[target_index]
    else:
        probs = probs[range( probs.shape[0] ), target_index[:,0]]

    return -np.mean(np.log(probs))


def softmax_with_cross_entropy(predictions, target_index):
    """
    Computes softmax and cross-entropy loss for model predictions,
    including the gradient

    Arguments:
      predictions, np array, shape is either (N) or (batch_size, N) -
        classifier output
      target_index: np array of int, shape is (1) or (batch_size) -
        index of the true class for given sample(s)

    Returns:
      loss, single value - cross-entropy loss
      dprediction, np array same shape as predictions - gradient of predictions by loss value
    """
    # TODO: Copy from the previous assignment
    index = 0
    if type(target_index) == int:
        index = np.asarray([target_index])
    else:
        index = target_index.copy()

    if index.ndim == 1 and index.size > 1:
        index = index.reshape(-1, 1)

    prob = softmax(predictions.copy())
    loss = cross_entropy_loss(prob, index)

    y = np.zeros_like(predictions)

    if len(index.shape) == 1:
        y[index] = 1
    else:
        y[range(y.shape[0]), index[:, 0]] = 1

    dprediction = prob - 1 * y

    if predictions.ndim == 2:
        dprediction = dprediction / predictions.shape[0]
    return loss, dprediction


class Param:
    """
    Trainable parameter of the model
    Captures both parameter value and the gradient
    """

    def __init__(self, value):
        self.value = value
        self.grad = np.zeros_like(value)


class ReLULayer:
    def __init__(self):
        pass

    def forward(self, X):
        # TODO: Implement forward pass
        # Hint: you'll need to save some information about X
        # to use it later in the backward pass

        self.indexes = np.where(X < 0)
        result = X.copy()
        result[self.indexes] = 0
        return result

    def backward(self, d_out):
        """
        Backward pass

        Arguments:
        d_out, np array (batch_size, num_features) - gradient
           of loss function with respect to output

        Returns:
        d_result: np array (batch_size, num_features) - gradient
          with respect to input
        """
        # TODO: Implement backward pass
        # Your final implementation shouldn't have any loops
        d_result = d_out.copy()
        d_result[self.indexes] = 0
        return d_result

    def params(self):
        # ReLU Doesn't have any parameters
        return {}


class FullyConnectedLayer:
    def __init__(self, n_input, n_output):
        self.W = Param(0.001 * np.random.randn(n_input, n_output))
        self.B = Param(0.001 * np.random.randn(1, n_output))
        self.X = None

    def forward(self, X):
        # TODO: Implement forward pass
        # Your final implementation shouldn't have any loops
        self.X = X
        return np.dot(X, self.W.value) + self.B.value

    def backward(self, d_out):
        """
        Backward pass
        Computes gradient with respect to input and
        accumulates gradients within self.W and self.B

        Arguments:
        d_out, np array (batch_size, n_output) - gradient
           of loss function with respect to output

        Returns:
        d_result: np array (batch_size, n_input) - gradient
          with respect to input
        """
        # TODO: Implement backward pass
        # Compute both gradient with respect to input
        # and gradients with respect to W and B
        # Add gradients of W and B to their `grad` attribute

        # It should be pretty similar to linear classifier from
        # the previous assignment

        self.W.grad = np.dot(self.X.T, d_out)  # dL/dW = dL/dZ * dZ/dW = X.T * dL/dZ
        self.B.grad = np.array([np.sum(d_out, axis=0)]) # dL/dB = dL/dZ * dZ/dB = I * dL/dZ

        gradX = np.dot(d_out, self.W.value.T) # dL/dX = dL/dZ * dZ/dX = dL/dZ * W.T
        return gradX

    def params(self):
        return {'W': self.W, 'B': self.B}

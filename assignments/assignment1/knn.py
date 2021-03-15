import numpy as np


class KNN:
    """
    K-neariest-neighbor classifier using L1 loss
    """
    def __init__(self, k=1):
        self.k = k

    def fit(self, X, y):
        self.train_X = X
        self.train_y = y

    def predict(self, X, num_loops=0):
        '''
        Uses the KNN model to predict clases for the data samples provided
        
        Arguments:
        X, np array (num_samples, num_features) - samples to run
           through the model
        num_loops, int - which implementation to use

        Returns:
        predictions, np array of ints (num_samples) - predicted class
           for each sample
        '''
        if num_loops == 0:
            dists = self.compute_distances_no_loops(X)
        elif num_loops == 1:
            dists = self.compute_distances_one_loop(X)
        else:
            dists = self.compute_distances_two_loops(X)

        if self.train_y.dtype == np.bool:
            return self.predict_labels_binary(dists)
        else:
            return self.predict_labels_multiclass(dists)

    def compute_distances_two_loops(self, X):
        '''
        Computes L1 distance from every sample of X to every training sample
        Uses simplest implementation with 2 Python loops

        Arguments:
        X, np array (num_test_samples, num_features) - samples to run
        
        Returns:
        dists, np array (num_test_samples, num_train_samples) - array
           with distances between each test and each train sample
        '''


        num_train = self.train_X.shape[0]
        num_test = X.shape[0]
        dists = np.zeros((num_test, num_train), np.float32)
        for i_test in range(num_test):
            for i_train in range(num_train):
                dists[i_test][i_train] = sum(abs(X[i_test] - self.train_X[i_train]))

        return  dists

    def compute_distances_one_loop(self, X):
        '''
        Computes L1 distance from every sample of X to every training sample
        Vectorizes some of the calculations, so only 1 loop is used

        Arguments:
        X, np array (num_test_samples, num_features) - samples to run
        
        Returns:
        dists, np array (num_test_samples, num_train_samples) - array
           with distances between each test and each train sample
        '''
        num_train = self.train_X.shape[0]
        num_test = X.shape[0]
        dists = np.zeros((num_test, num_train), np.float32)
        for i_test in range(num_test):
            s = abs((X[i_test].reshape(1,-1) - self.train_X)).sum(axis=1)

            dists[i_test] = s.T


        return dists

    def compute_distances_no_loops(self, X):
        '''
        Computes L1 distance from every sample of X to every training sample
        Fully vectorizes the calculations using numpy

        Arguments:
        X, np array (num_test_samples, num_features) - samples to run
        
        Returns:
        dists, np array (num_test_samples, num_train_samples) - array
           with distances between each test and each train sample
        '''

        dists = abs(X.reshape(-1,1, X.shape[1]) - self.train_X).sum(axis=2)

        return  dists

    def get_min(self,  dists, labels):
        unique, counts = np.unique(labels, return_counts=True)
        indexes = np.argsort(counts)

        d =[]
        l =[]

        label = unique[indexes][-1]

        ind = [i for i, x in enumerate(labels) if x == label]
        d.append(sum(dists[ind]))
        l.append(label)

        i = - 1
        while abs(i) < len(unique) and counts[indexes][i] == counts[indexes][i - 1]:
            label = unique[indexes][i - 1]
            ind = [i for i, x in enumerate(labels) if x == label]
            d.append(sum(dists[ind]))
            l.append(label)
            i = i - 1

        '''''
        print(d)
        print(l)
        print('------------------')
        '''
        return l[np.argsort(d)[0]]



    def predict_labels(self, dists, type):

        num_test = dists.shape[0]
        pred = np.zeros(num_test, type)
        for i in range(num_test):
            indexes = np.argsort(dists[i])[:self.k]

            labels = self.train_y[indexes]

            if self.k > 1:
                pred[i] = self.get_min(dists[i][indexes], labels)
            else:
                pred[i] = labels[0]
        return pred

    def predict_labels_binary(self, dists):
        '''
        Returns model predictions for binary classification case
        
        Arguments:
        dists, np array (num_test_samples, num_train_samples) - array
           with distances between each test and each train sample

        Returns:
        pred, np array of bool (num_test_samples) - binary predictions 
           for every test sample
        '''

        return self.predict_labels(dists, np.bool)

    def predict_labels_multiclass(self, dists):
        '''
        Returns model predictions for multi-class classification case
        
        Arguments:
        dists, np array (num_test_samples, num_train_samples) - array
           with distances between each test and each train sample

        Returns:
        pred, np array of int (num_test_samples) - predicted class index 
           for every test sample
        '''
        return self.predict_labels(dists, np.int)

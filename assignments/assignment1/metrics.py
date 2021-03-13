import numpy as np

def binary_classification_metrics(prediction, ground_truth):
    '''
    Computes metrics for binary classification

    Arguments:
    prediction, np array of bool (num_samples) - model predictions
    ground_truth, np array of bool (num_samples) - true labels

    Returns:
    precision, recall, f1, accuracy - classification metrics
    '''

    res = sum([a == b for a, b in zip(prediction, ground_truth)])
    accuracy = res / len(prediction)

    tp = sum([a == b and a == 1 for a, b in zip(prediction, ground_truth)])

    fn = sum([a != b and a == 0 for a, b in zip(prediction, ground_truth)])

    #tn = sum([a == b and a == 0 for a, b in zip(prediction, ground_truth)])

    fp = sum([a != b and a == 1 for a, b in zip(prediction, ground_truth)])

    precision = np.round(tp / (tp + fp),2)
    recall = np.round(tp / (tp + fn),2)

    f1 = 2 * (precision * recall / (precision + recall))



    # TODO: implement metrics!
    # Some helpful links:
    # https://en.wikipedia.org/wiki/Precision_and_recall
    # https://en.wikipedia.org/wiki/F1_score
    
    return precision, recall, f1, accuracy


def multiclass_accuracy(prediction, ground_truth):
    '''
    Computes metrics for multiclass classification

    Arguments:
    prediction, np array of int (num_samples) - model predictions
    ground_truth, np array of int (num_samples) - true labels

    Returns:
    accuracy - ratio of accurate predictions to total samples
    '''
    # TODO: Implement computing accuracy
    return 0

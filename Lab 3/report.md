# Tom Scallon
## Com S 472, Lab 3 Report

### House Dataset

The submitted .zip file contains the house dataset converted into an `.arff`
format. This was done by copying the original file, `house-votes-84.data` and
adding the necessary `arff` annotations to the beginning.

Afterward, Weka was used to obtain the naive Bayes classifier for the data.
The classification variable used was the party (Republican or Democrat). This
was done in Weka by the following procedure:

  1. Start Weka.
  2. Select `Explorer`.
  3. On the `Preprocess` tab, choose `Open file...` and select
     `house-votes-84.arff`.
  4. Switch to the `Classify` tab.
  5. Under the `Classifier` heading, click `Choose`. Select the naive Bayes
     classifier, located at `weka/classifiers/bayes/NaiveBayes`.
  6. Under the `Test Options` heading, select `Cross-Validation` and set
     `Folds` to `5`.
  7. Below the `Test Options` section, make sure `(Nom) party` is selected.
  8. Hit `Start`. The results are displayed to the right.

The important results obtained from the above procedure are copied below:

<pre>Correctly Classified Instances         392               90.1149 %
Incorrectly Classified Instances        43                9.8851 %

  a   b   &lt;-- classified as
154  14 |   a = republican
 29 238 |   b = democrat</pre>

As we can see, the naive Bayes classifier does fairly well at classifying the
House dataset, correctly labelling just over 90% of the individuals in the set
(using the cross-validation approach). (Note that this 90.1149% is merely an
estimate of the accuracy of the naive Bayes classifier.) The classifier seems
better at classifying Republicans correctly: only 14 Republicans are classified
as Democrats, while 29 Democrats are classified as Republicans.

### Breast Cancer dataset

#### Without Discretized Data

Weka was used to obtain the naive Bayes classifier for the data. The
classification variable used was the class (2 or 4). Much of the data from this
dataset is numeric; we first perform classification leaving the data as is.
This was done in Weka by the following procedure:

  1. Start Weka.
  2. Select `Explorer`.
  3. On the `Preprocess` tab, choose `Open file...` and select
     `breast-cancer-wisconsin.arff`.
  4. Switch to the `Classify` tab.
  5. Under the `Classifier` heading, click `Choose`. Select the naive Bayes
     classifier, located at `weka/classifiers/bayes/NaiveBayes`.
  6. Under the `Test Options` heading, select `Cross-Validation` and set
     `Folds` to `5`.
  7. Below the `Test Options` section, make sure `(Nom) class` is selected.
  8. Hit `Start`. The results are displayed to the right.

The important results obtained from the above procedure are copied below:

<pre>Correctly Classified Instances         672               96.1373 %
Incorrectly Classified Instances        27                3.8627 %

  a   b   &lt;-- classified as
436  22 |   a = 2
  5 236 |   b = 4</pre>

The naive Bayes classifier again performs admirably here: is correctly
classifies 672 out of the 699 cases, for a misclassification rate of just
3.8627%. The classifier more accurately classifies observations maligant tumors
(class 4): only 5 observations of malignant tumors are categorized as benign,
while 22 benign tumors classified as malignant. Note this isn't necessarily bad;
we would rather err on the safe side of classifying a benign tumor as malignant
than miss a malignant tumor and label it benign.

#### With Discretized Data

Next, we attempt the same analysis, except we use Weka's filter options to
discretize the numeric data into 10 bins. The process is the same as above,
except between steps 3 and 4 we do the following:

  1. Under the `Filter` header, click the `Choose` button.
  2. Select the filter `weka/filters/unsupervised/attribute/Discretize`.

<pre>Correctly Classified Instances         679               97.1388 %
Incorrectly Classified Instances        20                2.8612 %

  a   b   &lt;-- classified as
443  15 |   a = 2
  5 236 |   b = 4</pre>

Discretizing the data before running the classification marginally improves the
results; our misclassification rate is now just 2.8612%. We incorrectly classify
the same number of malignant tumors, but now only 15 benign tumors are
misclassified (instead of 22). As a result, discretizing is a valuable step for
this dataset -- and we will use these results of the non-discretized ones.

We also attempt setting the `useEqualFrequency` option to `true`; doing so
yields the same results as when it is set to `false`.

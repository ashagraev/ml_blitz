import sklearn.linear_model as lm
import math

x = []
y = []

for line in open('data.txt'):
    xx, yy = map(float, line.strip().split())
    x.append(xx)
    y.append(yy)

features = []
for xx in x:
    curFeatures = [
        math.sin(xx) ** 2,              # a^2
        math.log(xx) ** 2,              # b^2
        math.sin(xx) * math.log(xx),    # 2ab
        xx ** 2                         # c
    ]
    features.append(curFeatures)

linearModel = lm.LinearRegression()
linearModel.fit(features, y)

coeffs = linearModel.coef_

a = math.sqrt(coeffs[0])
b = math.sqrt(coeffs[1])
c = coeffs[3]

print "free coeff: ", linearModel.intercept_
print "2ab error: ", coeffs[2] - 2 * a * b
print a, b, c
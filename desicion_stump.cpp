#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>

using std::cin;
using std::cout;
using std::count;
using std::vector;
using std::sort;
using std::endl;
using std::fstream;

class TMeanCalculator {
private:
    double Mean = 0.;
    double SumWeights = 0.;
public:
    void Add(const double value, const double weight = 1.) {
        SumWeights += weight;
        if (SumWeights) {
            Mean += weight * (value - Mean) / SumWeights;
        }
    }

    double GetMean() const {
        return Mean;
    }

    double GetSumWeights() const {
        return SumWeights;
    }
};

class TSSECalculator {
private:
    double SumSquaredErrors = 0.;
    TMeanCalculator MeanCalculator;
public:
    void Add(const double value, const double weight = 1.) {
        const double lastMean = MeanCalculator.GetMean();
        MeanCalculator.Add(value, weight);

        const double sumWeights = MeanCalculator.GetSumWeights();
        if (!sumWeights) {
            return;
        }

        SumSquaredErrors += weight * (value - lastMean) * (value - MeanCalculator.GetMean());
    }

    void Remove(const double value, const double weight = 1.) {
        Add(value, -weight);
    }

    double GetMean() const {
        return MeanCalculator.GetMean();
    }

    double GetSumSquaredErrors() const {
        return SumSquaredErrors;
    }
};

struct TInstance {
    double Feature = 0.;
    double Target = 0.;

    static TInstance FromStream(fstream& in) {
        TInstance result;
        in >> result.Feature;
        in >> result.Target;
        return result;
    }

    bool operator < (const TInstance& other) const {
        return Feature < other.Feature;
    }
};

struct TStump {
    double Threshold = 0.;
    double LeftPrediction = 0.;
    double RightPrediction = 0.;
    double SumSquaredErrors = 0.;

    TStump(const TSSECalculator& splitSSECalculator)
        : LeftPrediction(splitSSECalculator.GetMean())
        , RightPrediction(splitSSECalculator.GetMean())
        , SumSquaredErrors(splitSSECalculator.GetSumSquaredErrors())
    {
    }

    TStump(const TSSECalculator& leftSSECalculator, const TSSECalculator& rightSSECalculator, const double threshold)
        : LeftPrediction(leftSSECalculator.GetMean())
        , RightPrediction(rightSSECalculator.GetMean())
        , SumSquaredErrors(leftSSECalculator.GetSumSquaredErrors() + rightSSECalculator.GetSumSquaredErrors())
        , Threshold(threshold)
    {
    }
};

class TStumpCalculator {
private:
    vector<TInstance> Instances;

    TSSECalculator SplitSSECalculator;

    bool Ready = false;
public:
    static TStumpCalculator FromStream(fstream& in) {
        TStumpCalculator result;

        int instancesCount = 0;
        in >> instancesCount;

        for (int instanceIdx = 0; instanceIdx < instancesCount; ++instanceIdx) {
            const TInstance instance = TInstance::FromStream(in);
            result.Instances.push_back(instance);
            result.SplitSSECalculator.Add(instance.Target);
        }

        result.Finish();
        return result;
    }

    TStump GetBestStump() const {
        assert(Ready);

        TStump bestStump(SplitSSECalculator);

        TSSECalculator leftCalculator;
        TSSECalculator rightCalculator = SplitSSECalculator;

        for (size_t movingInstanceIdx = 0; movingInstanceIdx + 1 < Instances.size(); ++movingInstanceIdx) {
            const TInstance& movingInstance = Instances[movingInstanceIdx];
            const TInstance& nextInstance = Instances[movingInstanceIdx + 1];

            leftCalculator.Add(movingInstance.Target);
            rightCalculator.Remove(movingInstance.Target);

            if (movingInstance.Feature + 1e-6 > nextInstance.Feature) {
                continue;
            }

            TStump currentStump(leftCalculator, rightCalculator, (movingInstance.Feature + nextInstance.Feature) / 2);
            if (currentStump.SumSquaredErrors < bestStump.SumSquaredErrors) {
                bestStump = currentStump;
            }
        }

        return bestStump;
    }
private:
    void AddInstance(const TInstance& instance) {
        Ready = false;
        Instances.push_back(instance);
    }

    void Finish() {
        sort(Instances.begin(), Instances.end());
        Ready = true;
    }
};

int main() {
    fstream in("stump.in", std::ios::in);
    fstream out("stump.out", std::ios::out);

    const TStumpCalculator stumpCalculator = TStumpCalculator::FromStream(in);
    const TStump stump = stumpCalculator.GetBestStump();

    out.precision(10);
    out << stump.LeftPrediction << " " << stump.RightPrediction << " " << stump.Threshold << endl;

    return 0;
}

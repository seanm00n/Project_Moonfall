// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Combat/CombatSystemLibrary.h"

float UCombatSystemLibrary::CalculateProbability_Bernoulli(int NumSuccesses, int NumTrials, float SuccessRate)
{
	// 베르누이 분포에서 성공할 확률 계산
	float P = SuccessRate;

	// 베이즈 정리를 이용하여 사후 확률 계산
	float Posterior = (float)NumSuccesses / (float)NumTrials;
	float Likelihood = powf(P, NumSuccesses) * powf(1 - P, NumTrials - NumSuccesses);
	float Evidence = Likelihood + (1 - Posterior) * powf(1 - P, NumTrials - NumSuccesses);
	return Likelihood / Evidence;
}

float UCombatSystemLibrary::CalculateProbability_EWMA(float Weight, float SuccessRate)
{
	SuccessRate = (1 - Weight) * SuccessRate + Weight * 1.0f; // EWMA 적용


	SuccessRate = FMath::Clamp(SuccessRate, 0.0f, SuccessRate * 2.0f); // 범위 제한

	return SuccessRate;
}

float UCombatSystemLibrary::CalculateProbability_BinomialDistribution(int32 NumTrials, float Probability, int32 NumSuccesses)
{
    float q = 1.f - Probability;
    float p = Probability;
    float k = (float)NumSuccesses;
    float n = (float)NumTrials;
    float choose = 1.f;
    float sum = 0.f;

    for (int32 i = 0; i <= k; i++)
    {
        sum += choose * powf(p, i) * powf(q, n - i);
        choose *= ((n - i) / (i + 1));
    }

    return sum;
}
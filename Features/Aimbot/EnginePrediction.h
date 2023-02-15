#pragma once
enum Prediction_stage
{
	SETUP,
	PREDICT,
	FINISH
};
class EnginePrediction
{
private:
	int iTickBaseBackup;
	int iFlagsBackup;
	int iButtonsBackup;
	int* m_pPredictionRandomSeed;
	int* m_pSetPredictionPlayer;

	//BackupData
	int flags = 0;
	//PredictionData
	void reset()
	{
		prediction_stage = SETUP;
		old_curtime = 0.0f;
		old_frametime = 0.0f;
	}
	Prediction_stage prediction_stage = SETUP;
	float old_curtime = 0.0f;
	float old_frametime = 0.0f;
	int* prediction_random_seed = nullptr;
	int* prediction_player = nullptr;
	//NetVarsData
	int tickbase = 0;
	//ViewModelData
	int viewmodel_index = 0;
	int sequence = 0;
	int animation_parity = 0;
	float cycle = 0.0f;
	float animation_time = 0.0f;

public:

	void Start();
	void End();
};

extern EnginePrediction g_EnginePrediction;
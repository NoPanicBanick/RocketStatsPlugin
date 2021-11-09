#include "pch.h"
#include "StatEventPlugin.h"
#include "Combaseapi.h"
#include <cpr/cpr.h>

BAKKESMOD_PLUGIN(StatEventPlugin, "Stat Event Plugin", "1.0", PERMISSION_ALL)

void StatEventPlugin::onLoad() {
	this->Log("Stat Event Plugin Loaded");
	this->LoadHooks(); // Events
}

void StatEventPlugin::onUnload() {

}

void StatEventPlugin::LoadHooks() {
	gameWrapper->HookEventWithCallerPost<GameEventWrapper>("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded",
		[this](GameEventWrapper caller, void* params, std::string eventName)
		{
			UploadGame(caller);
		});
}

void StatEventPlugin::Log(std::string msg) {
	cvarManager->log(msg);
}

void StatEventPlugin::UploadGame(GameEventWrapper caller) {
	if (!caller) {
		cvarManager->log("Failed to bind to GameEventWrapper");
	}
	GUID guid;
	CoCreateGuid(&guid);

	wchar_t* guidString;
	StringFromCLSID(guid, &guidString);
	std::wstring ws(guidString);
	std::string stringuid(ws.begin() +1, ws.end() - 1);

	cvarManager->log("GAME ID IS: " + stringuid);
	//auto gameMode = caller.GetGameMode();

	ArrayWrapper<PriWrapper> players = caller.GetPRIs();
	int *teamScores = new int[2];
	for (int count = 0; count < players.Count(); count++) {
		PriWrapper player = players.Get(count);
		TeamInfoWrapper team = player.GetTeam();

		// Gather Stats
		int teamNumber = team.GetTeamIndex();
		auto playerID = player.GetUniqueIdWrapper();
		int ballTouches = player.GetBallTouches();
		int goals = player.GetMatchGoals();
		int bumps = player.GetCarTouches();
		int assists = player.GetMatchAssists();
		int demos = player.GetMatchDemolishes();
		int ownGoals = player.GetMatchOwnGoals(); // For zlipp stats
		int saves = player.GetMatchSaves();
		int shots = player.GetMatchShots();
		int score = player.GetMatchScore();

		// Create JSON Request
		/*auto r = cpr::Post(cpr::Url{ "https://localhost:7035/api/playerstats" },
			cpr::Body{ 
				R"({ "PlayerID":")" + std::to_string(playerID.GetUID()) +
				R"(", "PlayerName":")" + player.GetPlayerName().ToString() +
				R"(", "TeamID":")" + std::to_string(teamNumber) +
				R"(", "Goals":")" + std::to_string(goals) +
				R"(", "Assists":")" + std::to_string(assists) +
				R"(", "Bumbs":")" + std::to_string(bumps) +
				R"(", "Demos":")" + std::to_string(demos) +
				R"(", "BallTouches":")" + std::to_string(ballTouches) +
				R"(", "OwnGoals":")" + std::to_string(ownGoals) +
				R"(", "Shots":")" + std::to_string(shots) +
				R"(", "Score":")" + std::to_string(score) +
				R"(", "Saves":")" + std::to_string(saves) + R"(" })"
			},
			cpr::Header{ { "Content-Type", "application/json" } });*/

		// Team Score set
		teamScores[teamNumber] = team.GetScore(); // We will probably repeat on setting the score but this is where I can find the wrapper for now
		/*auto r2 = cpr::Post(cpr::Url{ "https://localhost:7035/api/matchstats" },
			cpr::Body{
				R"({ "PlayerID":")" + std::to_string(playerID.GetUID()) +
				R"(", "PlayerName":")" + player.GetPlayerName().ToString() +
				R"(", "TeamID":")" + std::to_string(teamNumber) +
				R"(", "Goals":")" + std::to_string(goals) +
				R"(", "Assists":")" + std::to_string(assists) +
				R"(", "Bumbs":")" + std::to_string(bumps) +
				R"(", "Demos":")" + std::to_string(demos) +
				R"(", "BallTouches":")" + std::to_string(ballTouches) +
				R"(", "OwnGoals":")" + std::to_string(ownGoals) +
				R"(", "Shots":")" + std::to_string(shots) +
				R"(", "Score":")" + std::to_string(score) +
				R"(", "Saves":")" + std::to_string(saves) + R"(" })"
			},
			cpr::Header{ { "Content-Type", "application/json" } });*/

	}
	cvarManager->log("Team 1 Score: " + std::to_string(teamScores[0]) + ".  Team 2 Score: " + std::to_string(teamScores[1]));
}
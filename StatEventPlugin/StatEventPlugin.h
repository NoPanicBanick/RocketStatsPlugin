#pragma once
#include "cpr/cpr.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#pragma comment( lib, "pluginsdk.lib" )

template <typename T, typename std::enable_if<std::is_base_of<ObjectWrapper, T>::value>::type*>
void GameWrapper::HookEventWithCallerPost(std::string eventName,
	std::function<void(T caller, void* params, std::string eventName)> callback) 
{
	auto wrapped_callback = [callback](ActorWrapper caller, void* params, std::string eventName) {
		callback(T(caller.memory_address), params, eventName);
	};
	HookEventWithCaller<ActorWrapper>(eventName, wrapped_callback);
}

class StatEventPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	// Overridden Methods
	virtual void onLoad();
	virtual void onUnload();

	// Custom Methods
	virtual void LoadHooks();

private:
	virtual void Log(std::string msg);
	virtual void UploadGame(GameEventWrapper caller);
};


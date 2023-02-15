class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

class IGameConsole : public IBaseInterface
{
public:
	virtual void Activate() = 0;
	virtual void Initialize() = 0;
	virtual void Hide() = 0;
	virtual void Clear() = 0;
	virtual bool IsConsoleVisible() = 0;
	virtual void SetParent(int parent) = 0;
}; extern IGameConsole* g_pGameConsole;

#ifdef SABASAENGINE_EXPORTS
#define SABASAENGINE_API __declspec(dllexport)
#else
#define SABASAENGINE_API __declspec(dllimport)
#endif // !SABASAENGINE_EXPORTS

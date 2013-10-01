#define TRUE 1
#define FALSE 0
#define BOOL int
#define LPCSTR LPSTR
typedef char* LPSTR;
#define UINT int
#define PASCAL _stdcall
class CObject;

struct CRuntimeClass
{
	LPCSTR m_lpszClassName;
	int m_nObjectSize;
	UINT m_wSchema;
	CObject* (PASCAL* m_pfnCreateObject)();
	CRuntimeClass* m_pBaseClass;
	static CRuntimeClass* pFirstClass;
	CRuntimeClass* m_pNextClass;
};

struct AFX_CLASSINIT{AFX_CLASSINIT(CRuntimeClass* pNewClass);};

#define RUNTIME_CLASS(class_name)\
	(&class_name::class##class_name)

//#define DECLARE_DYNAMIC(class_name)\

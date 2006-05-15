// El siguiente bloque ifdef muestra la forma est�ndar de crear macros que facilitan 
// la exportaci�n de archivos DLL. Todos los archivos de este archivo DLL se compilan con el s�mbolo Q3BSP_EXPORTS
// definido en la l�nea de comandos. Este s�mbolo no se debe definir en ning�n proyecto
// que utilice este archivo DLL. De este modo, otros proyectos cuyos archivos de c�digo fuente incluyan el archivo
// interpreta que las funciones Q3BSP_API se importan de un archivo DLL, mientras que este archivo DLL interpreta los s�mbolos
// definidos en esta macro como si fueran exportados.
/*
#ifdef Q3BSP_EXPORTS
#define Q3BSP_API __declspec(dllexport)
#else
#define Q3BSP_API __declspec(dllimport)
#endif

// Clase exportada de q3bsp.dll
class Q3BSP_API Cq3bsp {
public:
	Cq3bsp(void);
	// TODO: agregar m�todos aqu�.
};

extern Q3BSP_API int nq3bsp;

Q3BSP_API int fnq3bsp(void);
*/




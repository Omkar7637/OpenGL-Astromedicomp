#include<stdio.h>

// Global typedef

typedef int MY_INT;

int main(void)
{
    // function prototype
    // MY_INT add(MY_INT, MY_INT);
    MY_INT Add(MY_INT , MY_INT );


    typedef int MY_INT;
    typedef float PVG_FLOAT;
    typedef char CHARACTER;
    typedef double MY_DOUBLE;

    // *** JUST LIKE IN Win32SDK !!!***
    typedef unsigned int UINT;
    typedef UINT HANDLE;
    typedef HANDLE HWND;
    typedef HANDLE HINSTANCE;

    // variable decalartions
    MY_INT a =10, i;
    MY_INT iArray[] = {9, 18, 27, 36, 45, 54, 63, 72, 81, 90 };

    PVG_FLOAT f_pvg = 30.9f;
    const PVG_FLOAT f_pbg_pi = 3.14f;
    CHARACTER ch = '*';
    CHARACTER chArray_01[] = "Hello";
    CHARACTER chArray_02[][10] = { "RTR", "Batch", "2024-2025"};

    MY_DOUBLE d = 8.041997;

    UINT uint = 3456;
    HANDLE handle = 987;
    HWND hwnd = 9876;
    HINSTANCE hInstance = 14466;

    // code
    printf("\n\n");
    printf("Type MY_INT variable a = %d\n", a);

    printf("\n\n");

    for(i = 0;i < (sizeof(iArray) / sizeof(int)); i++)
    {
        printf("Type MY_INT array variable iArray[%d] = %d\n", i, iArray[i]);
    }

    printf("\n\n");

    printf("Type PVG_FLOAT variable f = %f\n", f_pvg);
    printf("Type PVG_FLOAT constant f_pvg_pi = %f\n", f_pbg_pi);

    printf("\n\n");
    printf("Type MY_DOUBLE varibale d = %lf\n", d);

    printf("\n\n");
    printf("Type CHARATER variable ch = %c\n", ch);

    printf("\n\n");
    printf("Type CHARATER array variable chArray_01 = %s\n", chArray_01);

    printf("\n\n");
    for (i = 0; i < (sizeof(chArray_02) / sizeof(chArray_02[0])); i++)
    {
        printf("%s\t", chArray_02[i]);
    }
    printf("\n\n");

    printf("\n\n");
    printf("Type UINT variable unit = %u\n\n", uint);
    printf("Type HANDLE variable handle = %u\n\n", handle);
    printf("type HWND variable hwnd = %u\n\n", hwnd);
    printf("Type HINSTANCE variable hInstance = %u\n\n", hInstance);

    MY_INT x = 90;
    MY_INT y = 30;
    MY_INT ret;

    ret = Add(x, y);
    printf("ret = %d\n\n", ret);

    return(0);

}

MY_INT Add(MY_INT a, MY_INT b)
{
    // code
    MY_INT c;
    c = a + b;
    return(c);
}
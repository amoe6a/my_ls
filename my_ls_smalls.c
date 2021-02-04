#include "my_ls.h"
#define MY_NULL ((void*)0)

void errmsg()
{
    fprintf(stderr, "\nOptions\n-R\tlist subdirectories recursively, in ASCII order\n");
    fprintf(stderr, "-a\tlist subdirectories in ASCII order, including <.> files\n");
    fprintf(stderr, "-t\tlist last modified subdirectories first\n");
    return;
}

unsigned long my_strlen( const char *arr )
{
    int length = 0;
    while ( arr[length] != '\0' && arr[length] )
        length++;
    return length;
}
char* my_strcpy( char* param_1, const char* param_2 )
{
    int i;
    for ( i = 0; i < (int)my_strlen( param_2 ); i++ ) {
        param_1[i] = param_2[i];
    }
    param_1[ my_strlen( param_2 ) ] = '\0';
    return param_1;
}
char* my_strncpy( char* param_1, const char* param_2, int size )
{
    int i;
    for ( i = 0; i < size; i++ ) {
        param_1[i] = param_2[i];
    }
    param_1[size] = '\0';
    return param_1;
}
int my_strcmp( const char* param_1, const char* param_2 )
{
    int i = 0;
    while( i <= (int)my_strlen( param_2 ) ) {
        if( *( param_1 + i ) > *( param_2 + i ) )
            return 1;
        else if( *( param_1 + i ) < *( param_2 + i ) )
            return -1;
        else
            i++;
    }
    return 0;
}
char* my_strstr( const char* hay, const char* needl )
{
    int needl_indx = -1, check_found = 1;
    int i, j;
    if( hay == MY_NULL )
        return (char*) MY_NULL;
    else if( *needl == '\0' )
        return (char*) hay;
    else {
        for( i = 0; i < (int)my_strlen( hay ); i++ ) {
            if( hay[i] == *needl ) {
                check_found = 1;
                for( j = 0; j < (int)my_strlen( needl ); j++ ) {
                    if( hay[ i + j ] != needl[j] )
                        check_found = 0;
                }
                if( check_found ){
                    needl_indx = i;
                    break;
                }
            }
        }
    }
    if( needl_indx == -1 )
        return (char*) MY_NULL;
    return (char*) &hay[ needl_indx ];
}
int* my_int_memset( int *arr, int c, unsigned long len )
{
    int *dst = arr;
    while (len > 0) {
        *dst = c;
        dst++;
        len--;
    }
    return arr;
}
char* my_char_memset( char *arr, int c, unsigned long len )
{
    char *dst = arr;
    while (len > 0) {
        *dst = (unsigned char) c;
        dst++;
        len--;
    }
    return arr;
}
char* my_strcat( char *dst, const char *src )
{
    int end = 0;
    while( dst[end] != '\0' )
        end++;
    my_strncpy( &dst[end], src, my_strlen( src ) );
    return dst;
}
char* my_strncat( char *dst, const char *src, unsigned long sizen )
{
    int end = 0;
    while( dst[end] != '\0' )
        end++;
    my_strncpy( &dst[end], src, sizen );
    return dst;
}

void swap_Secs( void **first, void **second )
{
    void *tmp;
    tmp = *first; 
    *first = *second;
    *second = tmp;
}
void swap_strings( char *first, char *second )
{
    char tmp[MAXSIZE];
    my_char_memset( tmp, '\0', MAXSIZE );
    my_strcpy( tmp, first );
    my_strcpy( first, second );
    my_strcpy( second, tmp );
}
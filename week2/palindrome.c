
/** Algorithm & Complexity Analysis
 *  isPalindrome(A,len):
 *      Input  array A of n(len) characters
 *      Output boolean 0/1 of whether the input string is palindrome
 *  -----------------------------------------------------------------
 *  Statement                       # primitive operations
 *  -----------------------------------------------------------------
 *  i = 0
 *  j = len-1                         1
 *  while i < j do                    (n/2)+1
 *      if A[i] == A[j] then          3n/2
 *          i++;                      n/2
 *          j--;                      n/2
 *      end if
 *      else
 *          return 0                  1
 *      end else
 *  end while
 *  return 1                          1
 *  -----------------------------------------------------------------
 *  Total: 3n+3, which is O(n)
 **/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 128


bool isPalindrome(char const A[], int len){
    /** Starting from the first and the last element in the
     * string to the middle, while the two elements are not
     * equal in ASCII, break the loop and return 0(false)
     * or return 1(true) when the loop may successfully stop
     * in the middle of the string.
     **/
    int i=0;
    int j=len-1;

    while (i < j){
        if (A[i]==A[j]){
            i++;
            j--;
        }
        else{
            return 0;
        }
    }
    return 1;
}


int main() {
    /** Initialization
     * Hereby, arguments length and maxSize represent the length
     * of input string and maximum accepted length respectively.
     **/
    int length;
    int maxSize = SIZE;
    char array[maxSize];

    printf("Enter a word: ");       // Catch the input string
    scanf("%s",array);
    length = strlen(array);      // Get the length of the very string

    // Check if the input string is palindrome
    if(isPalindrome(array,length)){
        printf("yes");
    }
    else{
        printf("no");
    }

    return 0;
}

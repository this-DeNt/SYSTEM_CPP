#include <iostream>
#include <tchar.h>

void replaceSpacesWithTabs(_TCHAR* str)
{
    while (*str)
    {
        if (*str == _T(' ')) { *str = _T('\t'); }
        str++;
    }
}

void countCharacters(const _TCHAR* str, int& lettersCount, int& digitsCount, int& otherCount)
{
    while (*str)
    {
        if (_istalpha(*str))
        {

            lettersCount++;
        }
        else if (_istdigit(*str))
        {

            digitsCount++;
        }
        else { otherCount++; } 

        str++;
    }
}


int countWords(const _TCHAR* str)
{


    int wordCount = 0;
    bool inWord = false;

    while (*str)
    {
        if (_istspace(*str))
        {
            
            inWord = false;
        }
        else
        {
            
            if (!inWord)
            {
                wordCount++;
                inWord = true;
            }
        }

        str++;
    }

    return wordCount;
}


int countVowels(const _TCHAR* str)
{
    int vowelCount = 0;
    const _TCHAR vowels[] = _T("aeiouAEIOU"); 

    while (*str)
    {
        if (_tcschr(vowels, *str) != nullptr)
        {
            
            vowelCount++;
        }
        str++;
    }

    return vowelCount;
}


bool isPalindrome(const _TCHAR* str)
{

    int length = _tcslen(str); 

    
    int start = 0;
    int end = length - 1; 


    while (start < end)
    {
        
        if (str[start] != str[end]) { return false; }

       
        start++;
        end--;
    }

    return true;
}

void removeCharacter(_TCHAR* str, int position)
{
    int length = _tcslen(str);


    if (position >= 0 && position < length)
    {
        for (int i = position; i < length - 1; i++) { str[i] = str[i + 1]; }

        str[length - 1] = _T('\0');
    }
}

void removeAllOccurrences(_TCHAR* str, _TCHAR target)
{
    _TCHAR* pos = _tcschr(str, target);

    while (pos != nullptr)
    {
        
        _tcscpy(pos, pos + 1);
        pos = _tcschr(pos, target);
    }
}


void insertCharacter(_TCHAR* str, int position, _TCHAR character)
{
    int length = _tcslen(str);


    if (position >= 0 && position <= length)
    {
     
        for (int i = length; i > position; i--) { str[i] = str[i - 1]; }

        
        str[position] = character;
    }
}








//проверка
int _tmain(int argc, _TCHAR* argv[])
{

    _TCHAR inputString[] = _T("Htllo, Maya! My card 4441 6556 7940 1124. Bye!");

    //1
        //Меняем пробел на ТАБ
    replaceSpacesWithTabs(inputString);
    _tprintf(_T("spaces -> tabs:\n%s\n"), inputString);

    //2
        //кол во БУКВ, цыфр и чего то еще рандомного что будет в строке
    int lettersCount = 0;
    int digitsCount = 0;
    int otherCount = 0;

    countCharacters(inputString, lettersCount, digitsCount, otherCount);

    _tprintf(_T("Letters: %d\n"), lettersCount); //буквы
    _tprintf(_T("Digits: %d\n"), digitsCount); // цыфры
    _tprintf(_T("Other characters: %d\n"), otherCount); //рандомное

    //3
        //кол во СЛОВ
    int wordCount = countWords(inputString);
    _tprintf(_T("Number of words: %d\n"), wordCount);

    //4
        //ГЛАСНЫЕ
    int vowelCount = countVowels(inputString);
    _tprintf(_T("Number of vowels: %d\n"), vowelCount);

    //5
    // в две стороны читается одинаково
        //палиндромом
    if (isPalindrome(inputString))
    {
        _tprintf(_T("Yep\n"));
    }
    else { _tprintf(_T("No\n")); }

    //6
        //УДАЛЕНИЕ символа на позиции 2
    removeCharacter(inputString, 2);
    _tprintf(_T("after removing (position 2):\n%s\n"), inputString);

    //7
        // Удаление всех 'l'
    removeAllOccurrences(inputString, _T('l'));
    _tprintf(_T("String after removing all occurrences of 'e':\n%s\n"), inputString);


    //8
        //символа '-' на 8
    insertCharacter(inputString, 8, _T('-'));
    _tprintf(_T("inserting:\n%s\n"), inputString);



    return 0;
}


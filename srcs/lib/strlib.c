/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:51 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 09:05:45 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../kernel/memory/vmm/malloc/malloc.h"
#include "stddef.h"

/*
** Compute the length of a string
** Example: "ABCDE" -> 5
** @param const string s: the string we want to compute the length
** @return the length of said string
*/
size_t strlen(const char *str)
{
	size_t len = 0;
    if (!str)
        return (0);
	while (str[len])
		len++;
	return len;
}

/*
** Check if the character c is:
** - lowercase alpha (between 'a' and 'z')
** - uppercase alpha (between 'A' and 'Z')
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/
int		isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

/*
** Similar to ft_strchr, but return a int instead of char *
** Locate character in a string
** @param const string s: The string to search into
** @param int c: the character to search for
** @return 0 if c is at index 0, or -1 if the character is not found
** if the character is found, return the index of the character in the string
*/
int		strchri(const char *s, int c)
{
	size_t	n;

	n = 0;
	if (!s || !c)
		return (0);
	while (s[n])
	{
		if (s[n] == c)
			return (n);
		n++;
	}
	return (-1);
}

/*
** Similar to ft_memset, except we cannot choose what character we want to fill
** with
** Write N x 0 bytes on the given data S
** @param void *s: the given data to write to
** @param size_t n: how many 0 we want to write in s
*/
void	bzero(void *s, size_t n)
{
	int		i;
	char	*t;

	i = 0;
	t = (char *)s;
	while (i != (int)n)
		t[i++] = '\0';
}

/*
** Check if the character c is printable (between space and '~')
** For example, you cannot print the char nul, even if it is in the ascii table
** @param int c: the character we want to check
** @return return 1 if the character is matching, 0 if not
*/
int		isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

char	*strupper(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 97 && str[i] <= 122)
			str[i] -= 32;
		i++;
	}
	return (str);
}

/*
** Compare two strings and return the result
** @param const string s1: The 1rst string to compare
** @param const string s2: The 2nd string to compare
** @return -1 if one of the string is null, or the diference between them
*/
int		strcmp(char const *s1, char const *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
** Similar to ft_strcmp, but we only compare the n first bytes
** Compare two strings and return the result
** @param const string s1: The 1rst string to compare
** @param const string s2: The 2nd string to compare
** @param size_t n: the number of bytes to compare
** @return -1 if one of the string is null, 0 if n == 0,
**  or the diference between them
*/
int		strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i] && i < n - 1)
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
** Convert a string into a integer
** Example: "123" -> 123
** @param String str: the given string
** @return the string converted into a integer
*/
int		atoi(char const *str)
{
	size_t	output;
	int		i;
	int		mult;

	output = 0;
	i = 0;
	mult = 1;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' \
			|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		mult = (str[i] == '-') ? -1 : 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		output = output * 10 + str[i] - 48;
		i++;
	}
	return (output * mult);
}

/*
** Similar to ft_atoi, but return a long instead of a int
** Convert a string into a integer
** Example: "123" -> 123
** @param String str: the given string
** @return the string converted into a integer
*/

long	ft_atoi_long(char const *str)
{
    long	output;
    int		i;
    int		mult;

    output = 0;
    i = 0;
    mult = 1;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v' \
			|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        mult = (str[i] == '-') ? -1 : 1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        output = output * 10 + str[i] - 48;
        if (mult == 1 && output > MAX_INT64)
            return (-1);
        else if (mult == -1 && output > (long)(MAX_INT64))
            return (0);
        i++;
    }
    return (output * mult);
}


/*
** Copy n chars of strings from src to dest
** If chars already exist in dest, new characters will overwrite them
** @param string dest: The string to fill
** @param const string src: The string to fill with
** @return dest with the new datas copied from s2
*/
char	*strcpy(char *dest, char const *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*strlcpy(char *dest, char *src, int size)
{
	int i;

	i = 0;
	while (src[i] && i != size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*
** Create a new string by allocating memory and return it
** You need to FREE it after use
** @param size_t n: the size of the new string
** @return the string newly created
*/
char	*ft_strnew(size_t n)
{
	char	*dest;

	if (!(dest = (char *)malloc(sizeof(char) * (n + 1))))
		return (NULL);
	bzero(dest, n + 1);
	return (dest);
}

/*
** Split a string s by character c
** Example: "I love chocolate", ' ' -> "I", "love", "chocolate"
** Example: "This is a really good ingredient", 'i' -> "Th", "s ",
**   "s a really good ", "ngred", "ent"
** You need to FREE it after use, it is a double array
** @param string s: The string we want to split
** @param int c: the character we want to split with
** @return the double array containing splitted string
*/
static int		countwords(char const *s, char c)
{
	size_t	compteur;
	size_t	i;

	i = 0;
	compteur = 0;
	while (i < strlen(s))
	{
		if (i == 0 && s[i] != c)
			compteur++;
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			compteur++;
		i++;
	}
	return (compteur);
}

static int		return_i(char const *str, int i, char c)
{
	while (str[i] == c)
		i++;
	return (i);
}

static int		countchar(char const *str, int i, char c)
{
	int nbrchar;

	nbrchar = 0;
	i = return_i(str, i, c);
	while (str[i])
	{
		if (str[i] && str[i] != c)
			nbrchar++;
		else
			return (nbrchar);
		i++;
	}
	return (nbrchar);
}

char			**ft_strsplit(char const *s, char c)
{
	char	**ret;
	int		j;
	int		i;
	int		k;
	int		nbr_words;

	j = 0;
	i = 0;
	nbr_words = countwords(s, c);
	if (!s || !(ret = (char **)malloc(sizeof(char *) * (nbr_words + 1))))
		return (0);
	while (j < nbr_words)
	{
		i = return_i(s, i, c);
		k = 0;
		if (!(ret[j] = ft_strnew(countchar(s, i, c) + 1)))
			return (0);
		while (s[i] && s[i] != c)
			ret[j][k++] = s[i++];
		j++;
	}
	ret[j] = NULL;
	return (ret);
}

/*
** Similar to ft_isalnum, but for a string
** Check if the string str is:
** - numerical (between 0 and 9)
** - lowercase alpha (between 'a' and 'z')
** - uppercase alpha (between 'A' and 'Z')
** @param strings str: the string we want to check
** @return return 1 if the string is matching, 0 if not
*/
int			ft_strisalnum(char *str)
{
    int i;

    i = 0;
    while (str[i])
        if (!ft_isalnum(str[i++]))
            return (0);
    return (1);
}

/*
** Similar to ft_isalpha, but for a string
** Check if the string str is:
** - lowercase alpha (between 'a' and 'z')
** - uppercase alpha (between 'A' and 'Z')
** @param string str: the string we want to check
** @return return 1 if the string is matching, 0 if not
*/
int			ft_strisalpha(char *str)
{
    int i;

    i = 0;
    while (str[i])
        if (!ft_isalpha(str[i++]))
            return (0);
    return (1);
}

/*
** Similar to ft_isascii, but for a string
** Check if the string str is ascii (contained into the ascii table,
**  between 0 and 127)
** @param string str: the string we want to check
** @return return 1 if the string is matching, 0 if not
*/
int			ft_strisascii(char *str)
{
    int i;

    i = 0;
    while (str[i])
        if (!ft_isascii(str[i++]))
            return (0);
    return (1);
}

/*
** Check if a string is blank (constitued only by spaces or tabs)
** @param string str: The string we want to check
** @return 1 if the string match the requirements, and 0 if not
*/

int		ft_isblank(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t')
            return (0);
        ++i;
    }
    return (1);
}

/*
** Similar to ft_isdigit, but for string
** Check if the string str is a number (between 0 and 9)
** @param string str: the string we want to check
** @return return 1 if the string is matching, 0 if not
*/

int		ft_strisdigit(char *str)
{
    int i;

    i = 0;
    while (str[i])
        if (!ft_isdigit(str[i++]))
            return (0);
    return (1);
}

/*
** Similar to ft_strisdigit, but accept '-' or '+' as a first character
** Check if the string str is a number (between 0 and 9)
** @param string str: the string we want to check
** @return return 1 if the string is matching, 0 if not
*/
int		ft_strisnumeric(char *str)
{
    int i;

    i = 1;
    if (!str)
        return (0);
    if (!(ft_isdigit(str[0]) || str[0] == '+' || str[0] == '-'))
        return (0);
    while (str[i])
        if (!ft_isdigit(str[i++]))
            return (0);
    return (1);
}

/*
** Similar to ft_strcmp, but return int
** Compare two strings, and return a int if they are equals
** @param const string s1: the 1rst string to compare
** @param const string s2: the 2nd string to compare
** @return 1 if one of the strings is null, or if the strings are the same
**   return 0 if the strings are not equals
*/

int		ft_strequ(char const *s1, char const *s2)
{
    int i;

    i = 0;
    if (!s1 || !s2)
        return (1);
    while (s1[i] || s2[i])
    {
        if (s1[i] != s2[i])
            return (0);
        i++;
    }
    return (1);
}

/*
** Similar to ft_strequ, but specify a number of bytes to check
** Compare two strings, and return a int if they are equals
** @param const string s1: the 1rst string to compare
** @param const string s2: the 2nd string to compare
** @param size_t n: the number of bytes to check
** @return 1 if one of the strings is null, or if the strings are the same
**   return 0 if the string are not equals
*/

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
    int i;

    i = 0;
    if (!s1 || !s2)
        return (1);
    while ((s1[i] || s2[i]) && i < (int)n)
    {
        if (s1[i] != s2[i])
            return (0);
        i++;
    }
    return (1);
}

/*
** Allocate a substring of s, starting at index start,
**   and for length characters
** Example: "I love the dogecoin", 2, 17 -> "love the dogecoin"
** You need to FREE it after use
** @param const string s: the string you extract from
** @param unsigned int start: the starting index
** @param size_t len: The number of characters to extract
** @return the new string if everything suceed, or null if s is null, or malloc
**   fails
*/

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
    char	*ret;
    size_t	i;

    i = 0;
    if (!s)
        return (NULL);
    if (!(ret = (char *)malloc(sizeof(char) * (len + 1))))
        return (NULL);
    while (i < len)
    {
        ret[i] = s[start + i];
        ++i;
    }
    ret[i] = '\0';
    return (ret);
}

/*
** Duplicate a given string
** You need to FREE it after use
** @param const string s: the given string to duplicate
** @return a allocated string containing the old string,
** or NULL if malloc fails
*/

char	*ft_strdup(char const *s)
{
    char	*dest;
    int		i;

    if (!s || !(dest = (char *)malloc(sizeof(char) * strlen(s) + 1)))
        return (NULL);
    i = 0;
    while (s[i])
    {
        dest[i] = s[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

/*
** Remove the space and whitespaces from a string
** Example: "     I love my life while writing doc !       " ->
**   "I love my life while writing doc !"
** You need to FREE after use
** @param const string s: The string to trim
** @return the trimmed string
*/

char	*ft_strtrim(const char *s)
{
    int		i;
    int		len;

    i = 0;
    if (!s)
        return (0);
    len = strlen(s) - 1;
    while ((s[i] == '\t' || s[i] == ' ' || s[i] == '\n') && s[i] != '\0')
        i++;
    if (!s[i])
        return (ft_strdup(""));
    while ((s[len] == '\t' || s[len] == ' ' || s[len] == '\n') && len >= 0)
        len--;
    return (ft_strsub(s, i, len - i + 1));
}


/*
** Find a substring in a string
** Example: "I love chocolate", "love" -> "love chocolate"
** @param const string str: the string to search into
** @param const string to_find: the string we are looking for
** @return a pointer where the beginning of the located substring is
**  or 0 if not found
*/

char	*ft_strstr(char const *str, char const *to_find)
{
    int		i;
    int		j;

    i = 0;
    if (!str[0] && !to_find[0])
        return ((char *)str);
    if (!str[0] && to_find[0])
        return (0);
    while (str[i])
    {
        j = 0;
        while (str[i + j] == to_find[j] && to_find[j])
            j++;
        if (!to_find[j])
            return ((char *)str + i);
        i++;
    }
    if (!str[i] && !to_find[j])
        return (0);
    return (0);
}


/*
** Convert a string to lowercase, only uppercase letters
** Example: "ABC" -> "abc"
** @param string str: The string we want to convert
** @return the string converted into lowercase
*/

char	*ft_strlower(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] += 32;
        i++;
    }
    return (str);
}

/*
** Convert a string to upperrcase, only lowercase letters
** Example: "abc" -> "ABC"
** @param string str: The string we want to convert
** @return the string converted into uppercase
*/

char	*ft_strupper(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] >= 97 && str[i] <= 122)
            str[i] -= 32;
        i++;
    }
    return (str);
}

/*
** Concatenate 2 strings, but dest must have enought space in it, unlike
** ft_strjoin
** Example: "Abc", "def" -> "Abcdef"
** Add a \0 at the end
** @param string dest: the dest to copy into
** @param const string src: the second string that will be added
** @return the new concatenated string
*/

char	*ft_strcat(char *dest, char const *src)
{
    int i;
    int e;

    i = 0;
    e = 0;
    if (!dest || !src)
        return (dest);
    while (dest[i])
        i++;
    while (src[e])
    {
        dest[i + e] = src[e];
        e++;
    }
    dest[i + e] = '\0';
    return (dest);
}

/*
** Locate character in a string
** @param const string s: The string to search into
** @param int c: the character to search for
** @return the return a pointer to the matched character or
** NULL if the character is not found.
*/
char	*ft_strchr(char const *s, int c)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return ((char *)&s[i]);
        i++;
    }
    if (s[i] == '\0' && c == '\0')
        return ((char *)&s[i]);
    return (NULL);
}

/*
** Similar to ft_strcpy
** Copy 2 strings beginning at src[start]
** @param string dest: the dest string where we will copy into
** @param string src: the src string which we copy from
** @param int start: beginning to copy for start
** @return the dest with copied datas
*/

char	*ft_stricpy(char *dest, char *src, int start)
{
    int		i;

    i = 0;
    while (src[start])
        dest[i++] = src[start++];
    return (dest);
}

/*
** Similar to  ft_strcat
** Concatenate 2 strings beginning at src[start]
** @param string dest: the dest string where we will concantenate into
** @param string src: the src string which we concatenate from
** @param int start: beginning to copy for start
** @return the dest with copied datas
*/

char	*ft_stricat(char *dest, char *src, int start)
{
    int		i;
    int		e;

    i = 0;
    e = 0;
    while (dest[i])
        i++;
    while (src[start])
        dest[e + i++] = src[start++];
    dest[e + i] = '\0';
    return (dest);
}

/*
** Join 2 strings together using malloc
** Need to be FREED after used
** Example: "abc", "def" -> "abcdef"
** @param const string s1: The first string
** @param const string s2: The second string
** @return the new concatenated string s1 + s2
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
    char	*ret;
    int		i;
    int		e;

    if (!s1 || !s2)
        return (0);
    i = 0;
    e = 0;
    if (!(ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + \
						ft_strlen(s2)) + 1)))
        return (0);
    while (s1[i])
    {
        ret[i] = s1[i];
        i++;
    }
    while (s2[e])
    {
        ret[i + e] = s2[e];
        e++;
    }
    ret[i + e] = '\0';
    return (ret);
}

/*
** Similar to ft_strcat, but use size_t n to determinate the limit to copy
** Concatenate 2 strings, but dest must have enought space in it, unlike
** ft_strjoin
** Example: "Abc", "def" -> "Abcdef"
** Add a \0 at the end
** @param string dest: the dest to copy into
** @param const string src: the second string that will be added
** @param size_t n: the number of byte to concatenate
** @return the total length of the new string
*/
size_t	ft_strlcat(char *dest, char const *src, size_t n)
{
    unsigned int i;
    unsigned int buff;
    unsigned int srclen;

    buff = ft_strlen(dest);
    i = 0;
    srclen = 0;
    while (src[i] && buff + i + 1 < n)
    {
        dest[buff + i] = src[i];
        i++;
    }
    dest[buff + i] = '\0';
    srclen = ft_strlen(src);
    if (n < buff)
        return (srclen + n);
    return (buff + srclen);
}

/*
** Similar to ft_strcpy
** Copy size bytes from src to dest
** @param string dest: the destination string
** @param string src: the source string
** @param int size: the number of byte to copy from src
** @return the copy of the string
*/

char	*ft_strlcpy(char *dest, char *src, int size)
{
    int i;

    i = 0;
    while (src[i] && i != size)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}

/*
** Similar to ft_strcat, but will use the first n bytes of src
** Append the src string to the dest string.
** It overwrite the \0 of dest.
** @param string dest: the string we concatenate into
** @param const string src: the string we concatenate with
** @param size_t n: The number of byte to copy
** @return the copied string
*/

char	*ft_strncat(char *dest, const char *src, size_t n)
{
    size_t	i;
    int		len;

    i = 0;
    len = ft_strlen(dest);
    while (src[i] && i < n)
    {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
    return (dest);
}

/*
** Similar to ft_strcpy, but we specify a number of byte to copy
** Copy n chars of strings from src to dest
** If chars already exist in dest, new characters will overwrite them
** If length of src is smaller than n, the rest will be filled with \0
** @param string dest: The string to fill
** @param const string src: The string to fill with
** @param unsigned int n: The number of bytes to copy into dest
** @return dest with the new datas copied from s2
*/

char	*ft_strncpy(char *dest, char const *src, unsigned int n)
{
    unsigned int i;

    i = 0;
    while (src[i] && i < n)
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i++] = '\0';
    }
    return (dest);
}

/*
** Similar to ft_strdup, but you specify a number of bytes to copy
** Duplicate a given string
** You need to FREE it after use
** @param const string s: the given string to duplicate
** @param size_t n: the number of bytes to duplicate
** @return a allocated string containing the old string,
** or NULL if malloc fails
*/

char	*ft_strndup(const char *s, size_t n)
{
    unsigned int	i;
    char			*cpy;

    if (!(cpy = (char*)malloc(sizeof(char) * (n + 1))))
        return (NULL);
    i = 0;
    while (s[i] && i < n)
    {
        cpy[i] = s[i];
        i++;
    }
    cpy[i] = '\0';
    return (cpy);
}

/*
** Similar to ft_strstr, but you specify an index e to start looking
** Find a substring in a string
** Example: "I love chocolate", "love", 0 -> "love chocolate"
** Example: "I love chocolate", "love", 10 -> NULL
** @param const string str: the string to search into
** @param const string to_find: the string we are looking for
** @param size_t e: the index to start looking
** @return a pointer where the beginning of the located substring is
*/

char	*ft_strnstr(char const *s1, char const *s2, size_t e)
{
    int		i;
    int		j;

    i = 0;
    if (!s1[0] && !s2[0])
        return ((char *)s1);
    if (!s1[0] && s2[0])
        return (0);
    while (s1[i] && i < (int)e)
    {
        j = 0;
        while (s1[i + j] == s2[j] && s2[j] && (i + j < (int)e))
            j++;
        if (!s2[j])
            return ((char *)s1 + i);
        i++;
    }
    return (0);
}

/*
** Similar to ft_strchr, but we specify a starting index
** Locate character in a string
** Example: "I love chocolate", 10, 'c' -> 0
** @param const string s: The string to search into
** @param int start: the starting to start looking
** @param int c: the character to search for
** @return the return a pointer to the matched character or
** NULL if the character is not found.
*/

char	*ft_strrchr(char const *s, int start, int c)
{
    while (s[start])
    {
        if (s[start] == (char)c)
            return ((char *)&s[start]);
        start++;
    }
    return (0);
}

/*
** Similar to ft_strcpy, but we specify a index to start and
**   number of byte to copy
** Copy n chars of strings from src[i] to dest
** If chars already exist in dest, new characters will overwrite them
** If length of src is smaller than n, the rest will be filled with \0
** Example: "", "abcdefghi", 3, 2 -> "de"
** @param string dest: The string to fill
** @param const string src: The string to fill with
** @param unsigned int i: the index to start at src
** @param unsigned int n: The number of bytes to copy into dest
** @return dest with the new datas copied from src
*/

char	*ft_strnncpy(char *dest, char const *src, unsigned int i,
                     unsigned int n)
{
    int k;

    k = 0;
    while (src[i] && i < n)
    {
        dest[k] = src[i];
        i++;
        k++;
    }
    while (i < n)
    {
        dest[k++] = '\0';
        k++;
        i++;
    }
    return (dest);
}

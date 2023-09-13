/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bzero.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/22 14:18:09 by lubenard          #+#    #+#             */
/*   Updated: 2021/02/04 12:01:07 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/test_header.h"
#include <stdlib.h>

typedef struct s_books {
	char  title[50];
	char  author[50];
	char  subject[100];
	int   book_id;
}				t_books;

int		test_bzero_zero(void)
{
	char *string = "abc, def";
    bzero(string, 0);
	if (strlen(string) == 8)
		return (0);
	return (1);
}

int		test_bzero_malloced_str(void)
{
	char *string;
	int returnCode;

	if (!(string = malloc(sizeof(char) * 9)))
		return (2);
	strcpy(string, "abc, def\0");
    bzero(string, 8);
	if (strlen(string) == 8)
		returnCode = 1;
	returnCode = 0;

	free(string);
	return (returnCode);
}

int		test_bzero_struct() {
	t_books *books;
	int returnCode = 0;

	if (!(books = malloc(sizeof(t_books))))
		return (2);
	strcpy(books->title, "The house is very green");
	strcpy(books->author, "Me");
	strcpy(books->subject, "Me");
	books->book_id = 10;
    bzero(books, sizeof(t_books));
	if (strcmp(books->title, "\0") != 0)
		returnCode = 1;
	if (strcmp(books->author, "\0") != 0)
		returnCode = 1;
	if (strcmp(books->subject, "\0") != 0)
		returnCode = 1;
	if (books->book_id != 0)
		returnCode = 1;
	free(books);
	return (returnCode);
}

int test_bzero_partial()
{
	char *string;
	int returnCode = 0;

	if (!(string = malloc(sizeof(char) * 9)))
		return (2);
	strcpy(string, "abc, def\0");
	bzero(string, 3);
	if (strcmp(&(string[3]), ", def\0") != 0)
		returnCode = 1;
	free(string);
	return (returnCode);
}

void	launch_tests_bzero(void)
{
	int (*array_tests_addres[10])(void) = {test_bzero_zero, test_bzero_malloced_str, test_bzero_struct, test_bzero_partial};
	int counter;

	counter = 0;
	printf("Bzero: ");
	while (array_tests_addres[counter])
	{
		evaluate_tests(array_tests_addres[counter++]());
	}
	printf("\n");
}

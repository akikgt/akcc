// fizzbuzz test
/* this is test
this is test
this is test
*/
int main() {
    printf("fizzbuzz test\n");
    for (int i = 1; i <= 30; i += 1) {
        if (i % 3 == 0 && i % 5 == 0)
            printf("fizzbuzz\n");
        else if (i % 3 == 0)
            printf("fizz\n");
        else if (i % 5 == 0)
            printf("buzz\n");
        else
            printf("%d\n", i);
    }

    return 1;
}
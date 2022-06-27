#include<stdio.h>
#include<stdlib.h>
#include<string.h>
FILE * buy,*sell,*accounts;
void buy_order();
void sell_order();
void account();
int main()
{
    short int choice;
    while(1)
    {
        system("clear");
        printf("\t\t\t\t\tTrading Platform\n\n");
        printf("   Select from one of the following options:\n");
        printf("\t1.Place a Buy Trade\n\t2.Place a Sell Trade\n\t3.View tour Account\n\t4.exit\n\t\tEnter Your Choice:");
        scanf(" %hd",&choice);
        switch (choice)
        {
        case 1:
            buy_order();
            break;
        case 2:
            sell_order();
            break;
        case 3:
            account();
            break;
        case 4:
            system("clear");
            printf("\n\nExited the Platform succefully.....\n\n\n");
            exit(0);
            break;
        default:printf("Enter the correct choice (Enter any key to continue).\n");
            getchar();
            getchar();
        }
    }
    return(0);
}
void buy_order()
{
    float buy_price;
    int num_of_stocks;
    system("clear");
    printf("\n\nEnter the number of stocks you would like to place your buy order for: ");
    scanf("%d",&num_of_stocks);
    printf("Enter the desired price at which you would like to buy: ");
    scanf("%f",&buy_price);
    if((buy_price<=36.85) || (buy_price>=44.30))
    {
        printf("Enter Between the range of prices\n ");
        getchar();
        getchar();
        return;
    }
    buy = fopen("Live_user_orders.txt", "w");
    fprintf(buy,"%f %d b n",buy_price,num_of_stocks);
    fclose(buy);
}
void sell_order()
{
    float sell_price;
    int num_of_stocks;
    system("clear");
    printf("\n\nEnter the number of stocks you would like to place your sell order for: ");
    scanf("%d",&num_of_stocks);
    printf("Enter the desired price at which you would like to sell: ");
    scanf("%f",&sell_price);
    if(sell_price<=36.85 || sell_price>=44.30)
    {
        printf("Enter Between the range of prices\n ");
        getchar();
        getchar();
        return;
    }
    sell = fopen("Live_user_orders.txt", "w");
    fprintf(sell,"%f %d s n",sell_price,num_of_stocks);
    fclose(sell);
}
void account()
{
    system("clear");
    float amount=0;
    int qty=0;
    accounts = fopen("Accounts.txt","r+");
    printf("\t\t\t\tHOLDINGS:\n\n");
    printf("NAME\t\tQTY\t\t AMOUNT \n");
    while(1)
    {
        if(feof(accounts))
            break;
        fscanf(accounts,"%d %f\n",&qty,&amount);
        printf("ABC\t\t%d\t\t%0.3f\n",qty,(amount*qty));
    }
    fclose(accounts);
    getchar();
    getchar();
}
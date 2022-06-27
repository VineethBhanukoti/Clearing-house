#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
typedef struct
{
    float bid_offer;
    int order[100];
    int rear,front,total;
    char flag;
}trade;
trade *buy,*sell;
FILE *am,*as;//File pointer to open After Market Files
float upper_bound =  44.25,lower_bound = 36.90;
int size=((44.25-36.90)/0.05)+2;
void market_depth();
void file_read();
void match_order();
void look_or_add_order();
void delay(int);
int main()
{
    file_read();
    while(1)
    {
        system("clear");
        market_depth();
        match_order();
        look_or_add_order();
        delay(1000);
    }
}
void match_order()
{
    int index=0,buys,sells;
    char ch;
    while(index<=size)
    {
        if(((buy+index)->bid_offer==(sell+index)->bid_offer) && ((((sell+index)->rear-(sell+index)->front)!=0) && (((buy+index)->rear-(buy+index)->front)!=0)))
        {
            
            (buy+index)->front=((buy+index)->front+1)%99;
            (sell+index)->front=((sell+index)->front+1)%99;
            buys=(buy+index)->order[(buy+index)->front];
            sells=(sell+index)->order[(sell+index)->front];
            if(buys >= sells)
            {
                ch=(buy+index)->flag;
                (buy+index)->total-= sells;
                (sell+index)->total-= sells;
                buys-=sells;
                (buy+index)->order[(buy+index)->front]=buys;
                if(buys!=0)
                    (buy+index)->front--;
                if((ch == 'n'))
                {
                    as=fopen("Feedback.txt","w");
                    putc('y',as);
                    fclose(as);
                    as=fopen("Accounts.txt","a");
                    fprintf(as,"%d %f\n",sells+buys,((buy+index)->bid_offer));
                    fclose(as);
                }
                sells=0;
            }
            else
            {
                ch=(buy+index)->flag;
                (sell+index)->total-= buys;
                (buy+index)->total-= buys;
                sells-=buys;
                (sell+index)->order[(sell+index)->front]=sells;
                if(sells!=0)
                    (sell+index)->front--;
                if((ch == 'n'))
                {
                    as=fopen("Feedback.txt","w");
                    putc('y',as);
                    fclose(as);
                    as=fopen("Accounts.txt","a");
                    fprintf(as,"%d %f\n",buys+sells,(buy+index)->bid_offer);
                    fclose(as);
                }
                buys = 0;
            }   
            break;
        }
        index++;
    }
}
void market_depth()
{
    float amt1 = 44.25,amt2=36.90;
    int index=0,index2=0;
    printf("BUY\t\t     | SELL   \n");
    printf("Offer\tOrder\t Qty | BID\tOrder\tQty\n");
    for(int _=0;_<12;_++)
    {
        amt1-=0.05;
        for(;amt1>=lower_bound;amt1-=0.05)
        {
            index = ((amt1-lower_bound))/0.05;
            if(((buy+index)->rear-(buy+index)->front )!= 0)
            {
                printf("%0.2f\t  %d\t% 4d |",(buy+index)->bid_offer,((buy+index)->rear-(buy+index)->front),(buy+index)->total);
                break;
            }
        }
        amt2+=0.05;
        for(;amt2<=upper_bound;amt2+=0.05)
        {
            index2 = (amt2-(lower_bound))/0.05;
            if(((sell+index2)->rear-(sell+index2)->front)!=0)
            {
                printf(" %0.2f\t  %d\t% 4d \n",(sell+index2)->bid_offer,((sell+index2)->rear-(sell+index2)->front),(sell+index2)->total);
                break;
            }
        }
    }
}
void file_read()
{
    float amt,iter=0.0;
    int qty,index=0,pointer;
    buy = malloc(size*sizeof(trade));
    sell = malloc(size*sizeof(trade));
    for(iter = lower_bound;index < size;iter+=0.05,index++)
    {
        (buy+index)->bid_offer =iter;
        (buy+index)->rear=0;
        (buy+index)->total=0;
        (sell+index)->bid_offer =iter;
        (sell+index)->rear=0;
        (sell+index)->total=0;
    }
    am = fopen("AMBO.txt","r");
    while(1)
    {
        if(feof(am))
            break;
        fscanf(am,"%f %d",&amt,&qty);
        pointer = (((amt-lower_bound))/0.05)+1;
        (buy+pointer)->rear=((buy+pointer)->rear+1)%99;
        (buy+pointer)->order[(buy+pointer)->rear] = qty;
        (buy+pointer)->total+=qty;
    } 
    fclose(am);
    am = fopen("AMSO.txt","r");
    while(1)
    {
        if(feof(am))
            break;
        fscanf(am,"%f %d",&amt,&qty);
        pointer = ((amt-36.90)/0.05)+1;
        (sell+pointer)->rear=((sell+pointer)->rear+1)%99;
        (sell+pointer)->order[(sell+pointer)->rear] = qty;
        (sell+pointer)->total+=qty;
    } 
    fclose(am);
}
void look_or_add_order()
{
    float amt;
    int qty,fronts,rears,index; 
    char type,flag;
    am = fopen("Live_user_orders.txt","r");
    while(1)
    {
        if(feof(am))
            break;
        fscanf(am,"%f %d %c %c",&amt, &qty,&type,&flag);
        index = (((amt-lower_bound))/0.05)+1;
            if(type=='b')
            {
                as = fopen("Feedback.txt","w");
                rears=(buy+index)->rear;
                fronts = (buy+index)->front;
                if(rears == fronts)
                    (buy+index)->rear=(buy+index)->front=0;
                if(((fronts == 0) && (rears == 99)) || (fronts== (rears)+1))
                    fprintf(as,"Cannot Take Orders For that value");
                else if((qty>0) && (qty< 30000))
                {
                    (buy+index)->rear=((buy+index)->rear+1)%99;
                    (buy+index)->order[(buy+index)->rear] = qty;
                    (buy+index)->total+=qty;
                    (buy+index)->flag = 'n';
                    fclose(am);
                    am=fopen("Live_user_orders.txt","w");
                    fclose(am);
                    break;
                }
                fclose(as);
            }
            else if(type=='s')
            {
                as = fopen("Feedback.txt","w");
                rears=(sell+index)->rear;
                fronts = (sell+index)->front;
                if(rears == fronts)
                    (sell+index)->rear=(sell+index)->front=0;
                if(((fronts == 0) && (rears == 99)) || (fronts== (rears)+1))
                    fprintf(as,"Cannot Sell Orders For that value");
                else if((qty>0) && (qty< 30000))
                {
                    (sell+index)->rear=((sell+index)->rear+1)%99;
                    (sell+index)->order[(sell+index)->rear] = qty;
                    (sell+index)->total+=qty;
                    (sell+index)->flag = 'n';
                    fclose(am);
                    am=fopen("Live_user_orders.txt","w");
                    fclose(am);
                    break;
                }
                fclose(as);
            }
    }
    fclose(am);
}
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
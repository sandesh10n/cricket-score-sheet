#include<stdio.h>
#include<conio.h>
#include<string.h>

struct team{
    char team_name[25];
    struct player{
        char name[49];
        int runs;
        int balls_faced;
        int sixes;
        int fours;
        int runs_given;
        int balls_thrown;
        float overs;
        float strike_rate;
        float economy;
        int wickets;
        int bat_status;
        int ball_status;
        int out_status;
    }p[11];
    int extras;
    int total_runs;
    int total_wickets;
    int toss;
}t1,t2;

void initializePlayers(struct team *t) {
    // initializing intitial value to structure member
    for (int i = 0; i < 11; i++) {
        strcpy(t->p[i].name, "Unnamed");
        t->p[i].runs = 0;
        t->p[i].balls_faced = 0;
        t->p[i].sixes = 0;
        t->p[i].fours = 0;
        t->p[i].runs_given = 0;
        t->p[i].balls_thrown = 0;
        t->p[i].overs = 0.0;
        t->p[i].strike_rate = 0.0;
        t->p[i].economy = 0.0;
        t->p[i].wickets = 0;
        t->p[i].bat_status = 0;
        t->p[i].ball_status = 0;
        t->p[i].out_status = 0;
    }
    t->extras = 0;
    t->total_runs = 0;
    t->total_wickets = 0;
    t->toss=0;
}

int inputscore(){
    // returns what happens in each ball
    int ch,temp;

    printf("\n[1] for Legal Delivery \n");
    printf("[2] for Illegal Delivery(If Wide or No ball!!)\n");

    printf("Enter: ");
    scanf("%d",&ch);

    switch(ch){

        case 1:

            printf("0 for dotball\n1 for single\n2 for double\n3 for triples\n4 for four\n6 for sixes\n7 for wicket\n\n->");
            scanf("%d",&temp);

            do{

                if(temp>=0 && temp<=7){
                    return temp;
                    break;
                }

                printf("This Input Is Not Acceptable!!\nTry Again\n->");
                scanf("%d",&temp);

            }while(temp<0||temp>7);

            break;


        case 2:

            temp=-1;

            return temp;
            break;

        default:

            printf("\nError Input,Enter Again\n");
            inputscore();// if invalid input, again call function unless input if valid
    }
}

void strike_rotate(int *strike, int *non_strike){

    // swap strike and non strike value to rotate strike
    int temp;
    temp = *strike;
    *strike = *non_strike;
    *non_strike = temp;
}

int change_baller(struct team **t){

    //shows the list of baller to change, at end of the over
    int input=-99;

    printf("choose Baller\n");

    //display list of player
    for(int i=0;i<11;i++){
        printf("%d. %s\n",i+1,(*t)->p[i].name);
    }

    printf("\n->");
    scanf("%d",&input);

    while(input>11 || input<1){
        printf("WRONG INPUT!!");
        printf("\n->");
        scanf("%d",&input);
    }
    system("cls");

        (*t)->p[input-1].ball_status++;// update ball_status, either player has balled or not in the game
        return (input-1);

}

void innings(int totalover, struct team *tbat, struct team *tball) {

    // build inning, update structure member value, perform all major logic
    int target= tball->total_runs+1;
    int batsmanNo = 1;
    int strike=0;
    int non_strike=1;

    int wickets = 0;// count wickets to run the loop
    int ballsrem = totalover * 6;// count remaining balls to run loop

    int ballsplayed = 0;
    int ballerNo=change_baller(&tball);// initial baller


    // update bat_status, either player has batted or not in the game
    tbat->p[strike].bat_status++;
    tbat->p[non_strike].bat_status++;

    while (ballsrem > 0 && wickets < 11) {

        //display live score in console
        if (target!=1){
          printf("Target: %d\n\n",target);
        }
        printf("%s: %d/%d", tbat->team_name, tbat->total_runs, wickets);// batting team name, total runs and wickets
        printf("\nOver:%d.%d\n", (ballsplayed / 6), (ballsplayed % 6));// over balled

        printf("\n*%s:%d/%d",tbat->p[strike].name,tbat->p[strike].runs,tbat->p[strike].balls_faced);// on strike batsman
        printf("\n%s:%d/%d\n",tbat->p[non_strike].name,tbat->p[non_strike].runs,tbat->p[non_strike].balls_faced);// non striker batsman

        printf("\n%s:%d/%d\n\n",tball->p[ballerNo].name,tball->p[ballerNo].wickets,tball->p[ballerNo].runs_given);// current baller

        int balldec = inputscore();// result of each ball

        // update structure member value as per result
        if (balldec >= 0 && balldec <= 6) {

            tbat->p[strike].runs += balldec;// update batsman indivisual run
            tbat->p[strike].balls_faced++;// update ball faced by batsman
            tbat->total_runs += balldec;// update total run for team

            tball->p[ballerNo].runs_given += balldec;// update runs given by baller
            tball->p[ballerNo].balls_thrown++;// update legal ball thrown by baller

            ballsplayed++;
            ballsrem--;

            // strike rotates when odd runs is scored
            if (balldec%2!=0){
                strike_rotate(&strike,&non_strike);
            }

            if (balldec == 4) {
                tbat->p[strike].fours++;// update number of fours hit by batsman
            }

            if (balldec == 6) {
                tbat->p[strike].sixes++;// update number of sixes hit by batsman
            }
        }

        // for illegal deliveries
        if (balldec == -1) {
            tbat->total_runs++;// update total runs
            tball->p[ballerNo].runs_given++;// update run given by baller
            tball->extras++;// update extra runs
        }

        // for wickets
        if (balldec == 7) {

            tbat->p[strike].balls_faced++;

            tball->p[ballerNo].balls_thrown++;
            tball->p[ballerNo].wickets++;

            wickets++;
            ballsplayed++;
            ballsrem--;

            tbat->p[batsmanNo].out_status++;
            batsmanNo++;// change batsman
            strike=batsmanNo;// give strikes to new batsman
            tbat->p[batsmanNo].bat_status++;// update bat_status, that ensure batsman has entered in field

            tbat->total_wickets++;// update total wickets
        }
        system("cls");

        if (target!=1 && tbat->total_runs>=target){

                break;
        }


        // over changed
        if (ballsplayed % 6 == 0 && ballsrem!=0) {

            strike_rotate(&strike,&non_strike);// strike change
            ballerNo=change_baller(&tball);// baller change
        }

        system("cls");
    }
}

//print batting stats
void printBattingSummaryToTextFile(struct team *t, FILE *file) {

    // table heading
    fprintf(file, "Batting Performance of %s\n", t->team_name);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "| %-12s | %-6s | %-6s | %-6s | %-6s | %-10s|\n",
            "Player Name", "Runs", "Balls", "Fours", "Sixes", "Strike Rate");
    fprintf(file, "-----------------------------------------------------------------\n");


    for (int i = 0; i < 11; i++) {

        // checks batsman's, batted or not, if yes, print their stats
        if (t->p[i].bat_status != 0){

            float strike_rate = (float)t->p[i].runs / t->p[i].balls_faced * 100.0;

            if(t->p[i].out_status==0){
                fprintf(file,"| *%-11s | %-6d | %-6d | %-6d | %-6d | %-10.2f |\n",t->p[i].name, t->p[i].runs, t->p[i].balls_faced, t->p[i].fours, t->p[i].sixes, strike_rate);

            }

            else{
                fprintf(file,"|  %-11s | %-6d | %-6d | %-6d | %-6d | %-10.2f |\n",t->p[i].name, t->p[i].runs, t->p[i].balls_faced, t->p[i].fours, t->p[i].sixes, strike_rate);
            }

        }

        //else print did not bat
        else{
            fprintf(file,"|  %-11s | %-46s |\n",t->p[i].name,"did not bat");
        }

    }

    fprintf(file, "-----------------------------------------------------------------\n");
}

// print balling stats
void printBowlingSummaryToTextFile(struct team *t, FILE *file) {

    // table heading
    fprintf(file, "\nBowling Performance of %s\n", t->team_name);
    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file, "| %-11s | %-6s | %-6s | %-6s | %-6s |\n",
            "Player Name", "Wickets", "Runs", "Overs", "Economy");
    fprintf(file, "-----------------------------------------------------------------\n");

    for (int i = 0; i < 11; i++) {

        // print stats who has balled
        if (t->p[i].ball_status != 0){

            float overs = t->p[i].balls_thrown/6+ (t->p[i].balls_thrown%6)*0.1;
            float economy =((float)t->p[i].runs_given/t->p[i].balls_thrown)*6;

            fprintf(file, "| %-11s | %-7d | %-6d | %-6.1f | %-7.2f |\n",
                t->p[i].name, t->p[i].wickets, t->p[i].runs_given,overs,economy);
        }
    }

    fprintf(file, "-----------------------------------------------------------------\n");
}

//print match details
void printMatchSummaryToTextFile(struct team *t1, struct team *t2, FILE *file) {


    fprintf(file,"\n---> %s vs %s <---\n",t1->team_name,t2->team_name);
    fprintf(file, "___________________________FIRST INNING__________________________\n");

    // Print the batting summary for the first team
    printBattingSummaryToTextFile(t1, file);

    // Print the bowling summary for the second team
    printBowlingSummaryToTextFile(t2, file);

    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file,"TOTAL: %d/%d    EXTRA:%d\n",t1->total_runs,t1->total_wickets,t2->extras);
    fprintf(file, "-----------------------------------------------------------------\n");

    fprintf(file, "\n___________________________SECOND INNING_________________________\n");

    // Print the batting summary for the second team
    printBattingSummaryToTextFile(t2, file);

    // Print the bowling summary for the first team
    printBowlingSummaryToTextFile(t1, file);

    fprintf(file, "-----------------------------------------------------------------\n");
    fprintf(file,"TOTAL: %d/%d    EXTRA:%d\n",t2->total_runs,t2->total_wickets,t1->extras);
    fprintf(file, "-----------------------------------------------------------------\n");
}

void winner(){

     FILE *file=fopen("match_summary.txt","a");
    if(t1.total_runs>t2.total_runs){
        printf("%s wins the match!!!\n",t1.team_name);
        fprintf(file,"                     %s wins the match!!!\n",t1.team_name);
    }
    else if(t2.total_runs>t1.total_runs){
        printf("%s wins the match!!!\n",t2.team_name);
        fprintf(file,"                     %s wins the match!!!\n",t2.team_name);
    }
    else if(t1.total_runs=t2.total_runs){
        printf("Match tie\n");
    }
    else{
        printf("something went wrong :(");
    }
    fclose(file);
}

int main(){
    //welcome screen
    printf("*------------------------------------------------------*");
    printf("\n\t\tCRICKET SCORE SHEET\n");
    printf("*------------------------------------------------------*");
    printf("\n!-------------Welcome to Cricket Score Sheet-----------!");
    printf("\nA C program to keep record of your cricket match score..\n\nbuilt by:\nArun khadka\nPrabesh Phuyal\nSandesh Neupane");
    printf("\n\nPress any key to Create a Match !!\n");
    getch();
    system("cls");

    // initialization
    char team1[25],team2[25];
    int overs_to_play, first_batting_team;
    initializePlayers(&t1);
    initializePlayers(&t2);

    // match details
    printf("Enter No.of overs in an innings: ");
    scanf("%d", &overs_to_play);

    printf("Enter Name of Teams:\n");

    printf("Team1: ");
    scanf("%s", t1.team_name);

    printf("Team2: ");
    scanf("%s", t2.team_name);

    // Entering playing XI

    // for team 1
    printf("Enter Playing XI of %s:\n",t1.team_name);

    for(int i=0;i<11;i++){
        printf("%d. ",i+1);
        scanf("%s",t1.p[i].name);
    }

    // for team 2
    printf("Enter Playing XI of %s:\n",t2.team_name);

    for(int i=0;i<11;i++){
       printf("%d. ",i+1);
       scanf("%s",t2.p[i].name);
    }

    printf("Press any key to continue...");
    getch();
    system("cls");

    // toss
    printf("Which Team Bats First:\n[1] %s\n[2] %s\n", t1.team_name, t2.team_name);

    do{
    printf("-> ");
    scanf("%d", &first_batting_team);

    if(first_batting_team !=1 && first_batting_team !=2)
        printf("Invalid Input....\n");

    if(first_batting_team ==1 ||first_batting_team ==2)
        break;

    }while(first_batting_team !=1 ||first_batting_team !=2 );

    printf("Press any key to begin the match!!!");
    getch();
    system("cls");

    // First innings
    if (first_batting_team == 1){
        t1.toss++;
        innings(overs_to_play, &t1, &t2);
    }
    else{
        t2.toss++;
        innings(overs_to_play, &t2, &t1);
    }
    // inning break
    system("cls");
    printf("First Inning Ended, \nPress any key to continue Second Inning---> ");
    getch();
    system("cls");

    // Second innings
    if (first_batting_team == 1)
        innings(overs_to_play, &t2, &t1);

    else
        innings(overs_to_play, &t1, &t2);

    // Printing match summary
    FILE *file = fopen("match_summary.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return 1;
    }

    fprintf(file, "-----------------------SCORECARD---------------------------------\n");

    if (t1.toss>t2.toss)
        printMatchSummaryToTextFile(&t1, &t2, file);

    if (t2.toss>t1.toss)
        printMatchSummaryToTextFile(&t2, &t1, file);

    fclose(file);

    winner();

    printf("\nCheck 'match_summary.txt' for match Details\n");
    getch();

    return 0;
}



static int lossCount = 0;
static bool strat = false;
rps player_jnull1(int round,rps *myhist,rps *opphist) {
	time_t t;
   	if (round==0) srand((unsigned) time(&t));
	int random = rand() % 10 + 1;
	int i = 0;
	int oppRock = 0;
	int oppPaper = 0;
	int oppScissors = 0;
	int min = 0;
	int min2 = 0;
	lossCount = 0;
	//if(round == 49 && eval_round(myhist[48], opphist[48], NULL,NULL) == 0)
		//printf("I won");
	while(i < 50 && opphist[i] != Unknown){		
		if(opphist[i] == Rock)
			oppRock++;
		else if(opphist[i] == Paper)
			oppPaper++;
		else
			oppScissors++;
		i++;
	}

	i = round - 5;
	if(round >= 5){
		while(i < round && opphist[i] != Unknown){
			//printf("%d ",i);
			if(eval_round(myhist[i],opphist[i], NULL, NULL) == 1)
				lossCount++;
			if(opphist[i] == Rock){oppRock += 3;}
			if(opphist[i] == Paper){oppPaper += 3;}
			else{oppScissors += 3;}
			i++;
		}
	}
	//printf("Rocks: %d", oppRock);
	//printf("Paper: %d", oppPaper);
	//printf("Scissors: %d", oppScissors);
	if(oppRock < oppPaper) 
		min = oppRock;
	else 
		min = oppPaper;
	if(oppScissors < min){ 
		min2 = min; 
		min = oppScissors;
	}
	else if(min == oppPaper){
		if(oppScissors < oppRock)
			min2 = oppScissors;
		else
			min2 = oppRock;
	}
	if(min2 < min){
		if(min == oppRock) min2 = oppPaper;
		if(min == oppPaper) min2 = oppRock;
		else min2 = oppScissors;
	}
	//printf("min: %d", min);
	//printf("min2: %d", min2);
	//printf("Random: %d", random);
	//printf("Loss count: %d ", lossCount);
	if(lossCount == 5)
		strat = !strat;

	if(strat == false){
		if(random < 3){
			if(min != oppRock && min2 != oppRock) return Paper;
			if(min != oppPaper && min2 != oppPaper) return Scissors;
			else return Rock;
		}
		if(random > 2 && random < 6){
			if(min2 == oppRock) return Paper;
			if(min2 == oppPaper) return Scissors;
			else return Rock;
		}
		if(random > 5){
			if(min == oppRock) return Paper;
			if(min == oppPaper) return Scissors;
			else return Rock;
		}
	}
	else if(strat){
		if(opphist[round -1] == Rock) return Paper;
		if(opphist[round -1] == Paper) return Scissors;
		if(opphist[round -1] == Scissors) return Rock;
	}
	return Rock;
}

register_player(player_jnull1,"jnull1");

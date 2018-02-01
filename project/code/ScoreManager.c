#include "ScoreManager.h"

void init_scores(Score* Scores[])
{
	Score * insertScore = new_score();

	unsigned int i;

	for(i = 0; i <= 4; i++)
		Scores[i] = insertScore;
}

Score * new_score()
{
	Score * insertScore = (Score*) malloc(sizeof(Score));

	insertScore->Score = 0;
	insertScore->Day = 1;
	insertScore->Month = 1;
	insertScore->Year = 1;

	return insertScore;
}

void add_score(Score* Scores[], Score * s)
{
	Scores[4] = s;

	unsigned int i;

	for(i = 3; i != -1; i--)
	{
		if(Scores[i+1]->Score > Scores[i]->Score)
		{
			Score * sAux = new_score();

			sAux = Scores[i];
			Scores[i] = Scores[i+1];
			Scores[i+1] = sAux;
		}
	}
}

void write_scores(Score* Scores[])
{
	FILE *fp;

	fp = fopen("/home/lcom/lcom1516-t6g15/project/code/high_scores.txt", "w");

	unsigned int i;

	for(i = 0; i <= 4; i++)
		fprintf(fp, "%d %d %d %d \n", Scores[i]->Score, Scores[i]->Day, Scores[i]->Month, Scores[i]->Year);


	fclose(fp);
}

void read_scores(Score* Scores[])
{
	FILE *fp;

	fp = fopen("/home/lcom/lcom1516-t6g15/project/code/high_scores.txt", "r");

	unsigned int i = 0;
	for(i = 0; i <= 4; i++)
	{
		Score * readScore = new_score();

		fscanf(fp, "%d %d %d %d \n", &readScore->Score, &readScore->Day, &readScore->Month, &readScore->Year);

		add_score(Scores, readScore);
	}

	fclose(fp);

}

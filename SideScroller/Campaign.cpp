#include "Campaign.h"
#include <fstream>
#include <vector>

CCampaign::CCampaign(string campaignFilePath, SDL_Renderer *r, SDL_Window *w, int resX, int resY)
{
	std::fstream reader(campaignFilePath);
	while (!reader.eof())
	{
		char temp;
		reader.read(&temp, 1);
		campaignFile += temp;
	}
	reader.close();
	renderer = r; 
	window = w;
	resolution[0] = resX;
	resolution[1] = resY;
}


CCampaign::~CCampaign()
{
}

void CCampaign::RenderTextScene(string instructions)
{
	SDL_Color fontColor;
	fontColor.r = 255;
	fontColor.g = 255;
	fontColor.b = 255;
	fontColor.a = 255;
	SDL_Color bgColor;
	bgColor.r = 0;
	bgColor.g = 0;
	bgColor.b = 0;
	bgColor.a = 255;
	std::vector<string> rows;
	string row = "";
	int counter = 0;
	while (row != "eof")
	{
		rows.push_back(row);
		row = GetRow(instructions, counter);
		counter++;
	}
	
	int duration = 5000;
	for (int i = 0; i < rows.size(); i++)
	{
		int lastWordEnd = 0;
		std::vector<string> words;
		for (int x = 0; x < rows[i].size(); i++)
		{			
			if (rows[i].at(x) == ' ')
			{
				words.push_back(rows[i].substr(lastWordEnd, x - lastWordEnd));
				lastWordEnd = x;
			}
		}
		if (words[0] == "NewLine ")
		{
			TextSceneRow * temp = new TextSceneRow();
			temp->id = words[1];
			temp->text = "";
			for (int x = 2; x < words.size(); x++)
			{
				temp->text += words[x];
			}
			textMap[temp->id] = temp;
		}
		if (words[0] == "Attributes ")
		{
			TextSceneRow * temp = textMap[words[1]];
			
			for (int i = 2; i < words.size(); i++)
			{
				if (words[i].substr(0, 6) == "Fadein")
				{
					temp->fadeIn = std::stoi(words[i + 1]);
				}
				else if (words[i].substr(0, 6) == "Fadeou")
				{
					temp->fadeOut = std::stoi(words[i + 1]);
				}
				else if (words[i].substr(0, 3) == "App")
				{
					temp->appearAt = std::stoi(words[i + 1]);
				}
				else if (words[i].substr(0, 3) == "Dis")
				{
					temp->disappearAt = std::stoi(words[i + 1]);
				}
			}
		}
		if (words[0] == "Duration ")
		{
			duration = stoi(words[1]);
		}
	}
	
	int initialTime = SDL_GetTicks();
	while (SDL_GetTicks() - initialTime <= duration)
	{
		SDL_Delay(15);
		// snark.
	}

}

void CCampaign::LoadLevel(int lvlNumber)
{
	bool nextLevel = false;
	CGame game(GetRow(campaignFile, lvlNumber), &nextLevel, renderer, window, resolution[0], resolution[1], "devsave", lvlNumber);
	if (nextLevel)
	{
		LoadLevel(lvlNumber + 1);
	}
	else
	{
		LoadLevel(lvlNumber);
	}
}

string CCampaign::GetRow(const string & file, int rownumber)
{
	int rowCounter = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (rowCounter == rownumber)
		{
			int counter = 0;
			int tally = 0;
			string row;
			while (true)
			{
				if (i + counter < file.size())
				{
					if (file.at(i + counter) != '\n')
					{
						counter++;
					}
					else
					{
						row = file.substr(i, counter);
						return row;
					}
				}
				else return "eof";
			}
		}
		if (file.at(i) == '\n')
		{
			rowCounter++;
		}
	}
	return "eof";
}
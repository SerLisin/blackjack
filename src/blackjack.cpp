#include <iostream>
#include <vector>

using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::vector;


enum suit { CLUBS, DIAMONDS, SPADES, HEARTS };
enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};


class Card{

public:
//	enum suit { CLUBS, DIAMONDS, SPADES, HEARTS };
//	enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
    Card(rank r = ACE, suit s = SPADES, bool ifu = true);
    int GetValue() const;
    void Flip();
private:
    suit m_Suit;
    rank m_Rank;
    bool m_IsFaceUp;
};
Card::Card(rank r, suit s, bool p): m_Suit(s),m_Rank(r), m_IsFaceUp(p){}
void Card::Flip(){m_IsFaceUp = !(m_IsFaceUp);}
int Card::GetValue() const {
	int value = 0;
	if(m_IsFaceUp)
	{
		value = m_Rank;
		if(value > 10)
		{
			value = 10;
		}
	}
	   return value;
}

class Hand{
protected:
	vector<Card*> m_Cards;
public:
	Hand(){m_Cards.reserve(7);}
	void Add(Card* pCard){m_Cards.push_back(pCard);}
	void Clear()
	{
		vector<Card*>::iterator iter = m_Cards.begin();
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		{
			delete *iter;
			*iter = 0;
		}
    	m_Cards.clear();
	}
	int GetTotal() const{
		if(m_Cards.empty())
		{
			return 0;
		}
		int total = 0;
		vector<Card*>::const_iterator iter;
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		{
			total += (*iter)->GetValue();
		}
		bool containsAce = false;
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		{
			if ((*iter)->GetValue() == 1)//Card::ACE)
			{
				containsAce = true;
			}
		}
		if (containsAce && total <= 11)
		{
			total += 10;
		}
    	return total;
	}
	virtual ~Hand(){Clear();}
};

// Task4
class GenericPlayer : public Hand
{
protected:
	string name;
private:
	GenericPlayer(string n): name(n) {};
	virtual bool IsHitting() = 0;
	bool IsBoosted() const {return (GetTotal() > 21) ? true : false; }
	void Bust() const {cout << "Уважаемый " << name << ", у вас перебор!" << endl;}
	virtual ~GenericPlayer(){};
};

int main(int argc,char** args){

    cout << "\t\tWelcome to Blackjack!\n\n";
    
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        cout << "How many players? (1 - 7): ";
        cin >> numPlayers;
    }
    
    vector<string> names;
    string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        cout << "Enter player name: ";
        cin >> name;
        names.push_back(name);
    }
    cout << endl;
    
    // игровой цикл
/*    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> again;
    }
  */  

    Card *pCard1 = new Card{THREE,DIAMONDS,true};
	Card *pCard2 = new Card{ACE,DIAMONDS,true};
	Hand hand;
	hand.Add(pCard1);
	hand.Add(pCard2);
	cout << hand.GetTotal() << endl;

    return 0;
}

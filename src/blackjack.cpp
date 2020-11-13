#include <iostream>
#include <vector>

using namespace std;


//enum suit { CLUBS, DIAMONDS, SPADES, HEARTS };
//enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};


class Card{

public:
	enum suit { CLUBS, DIAMONDS, SPADES, HEARTS };
	enum rank {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
    Card(rank r = ACE, suit s = SPADES, bool ifu = true);
    int GetValue() const;
    void Flip();
	friend ostream& operator<< (ostream& os,const Card& aCard);
private:
    suit m_Suit;
    rank m_Rank;
    bool m_IsFaceUp;
};

Card::Card(rank r, suit s, bool p): m_Suit(s),m_Rank(r), m_IsFaceUp(p){}

void Card::Flip(){m_IsFaceUp = !(m_IsFaceUp);}

ostream& operator<< (ostream& os,const Card& aCard)
{
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
    const string SUITS[] = { "c", "d", "h", "s" };
    
    if (aCard.m_IsFaceUp)
    {
        os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
    }
    else
    {
        os << "XX";
    }
    
    return os;
}

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
			*iter = nullptr;
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
			if ((*iter)->GetValue() == Card::ACE)
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


class GenericPlayer : public Hand
{
friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer); // Task5
protected:
	string m_Name;
public:
	GenericPlayer(const string& name = ""): m_Name(name){};
	virtual bool IsHitting() const = 0;
	bool IsBoosted() const {return (GetTotal() > 21);}
	void Bust() const {cout << m_Name << " busts." << endl;}
	virtual ~GenericPlayer(){};
};

// Task5
ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";
    vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();pCard != aGenericPlayer.m_Cards.end();++pCard)
        {
            os << *(*pCard) << "\t";
        }
        if (aGenericPlayer.GetTotal() != 0)
        {
            cout << "(" << aGenericPlayer.GetTotal() << ")";
        }
    }
    else
    {
        os << "<empty>";
    }
    return os;
}

// Task3
class Player : public GenericPlayer
{
public:
	Player(const string& name = "");
	virtual ~Player();
	virtual bool IsHitting() const
	{
		cout << m_Name << ", do you want a hit? (Y/N): ";
		char response;
		cin >> response;
		return (response == 'y' || response == 'Y');
	};
	void Win() const {cout << m_Name << " wins.\n";};
	void Lose() const {cout << m_Name << " loses.\n";};
	void Push() const {cout << m_Name << " pushes.\n";};
};
// Task4
class House : public GenericPlayer
{
	House(const string& name = "House");
    virtual ~House();
    virtual bool IsHitting() const {return (GetTotal() <= 16);};
    void FlipFirstCard()
	{
		if (!(m_Cards.empty()))
		{
			m_Cards[0]->Flip();
		}
		else
		{
			cout << "No card to flip!\n";
		}
	};
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


    return 0;
}

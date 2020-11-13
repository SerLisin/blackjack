#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>

using namespace std;



//////////////////////////// class Card //////////////////////////////////

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

//////////////////////////// class Hand //////////////////////////////////

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

//////////////////////////// class GenericPlayer //////////////////////////////////

class GenericPlayer : public Hand
{
protected:
	string m_Name;
public:
	GenericPlayer(const string& name = ""): m_Name(name){};
	virtual bool IsHitting() const = 0;
	bool IsBoosted() const {return (GetTotal() > 21);}
	void Bust() const {cout << m_Name << " busts." << endl;}
	virtual ~GenericPlayer(){};
	friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer); 
};


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

//////////////////////////// class Player //////////////////////////////////

class Player : public GenericPlayer
{
public:
	Player(const string& name = ""):GenericPlayer(name){}
	virtual ~Player(){}
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

//////////////////////////// class House //////////////////////////////////

class House : public GenericPlayer
{
public:
	House(const string& name = "House"):GenericPlayer(name){}
    virtual ~House(){}
    virtual bool IsHitting() const {return (GetTotal() <= 16);}
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

//////////////////////////// class Deck //////////////////////////////////

// Task3
class Deck : public Hand
{
public:
    Deck();
    virtual ~Deck();
    void Populate();
    void Shuffle();
    void Deal(Hand& aHand);
    void AdditionalCards(GenericPlayer& aGenericPlayer);
};

Deck::Deck()
{
    m_Cards.reserve(52);
    Populate();
}

Deck::~Deck()
{}

void Deck::Populate()
{
    Clear();
	for (int s = Card::CLUBS; s <= Card::HEARTS; ++s)
    {
        for (int r = Card::ACE; r <= Card::KING; ++r)
        {
	        Add(new Card(static_cast<Card::rank>(r),
                         static_cast<Card::suit>(s)));
        }
    }
}

void Deck::Shuffle()
{
    random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& aHand) // polymorphism
{
    if (!m_Cards.empty())
    {
        aHand.Add(m_Cards.back());
        m_Cards.pop_back();
    }
    else
    {
        cout << "Out of cards. Unable to deal." << endl;
		exit(0);
    }
}

void Deck::AdditionalCards(GenericPlayer& aGenericPlayer) // polymorphism
{
    cout << endl;
    while (!(aGenericPlayer.IsBoosted()) && aGenericPlayer.IsHitting())
    {
        Deal(aGenericPlayer);             // polymorphism
		
        cout << aGenericPlayer << endl;

        
        if (aGenericPlayer.IsBoosted())
        {
            aGenericPlayer.Bust();
        }
    }
}

//////////////////////////// class Game //////////////////////////////////

// Task4
class Game
{
public:
    Game(const vector<string>& names);
    ~Game();
    void Play();
 private:
    Deck m_Deck;
    House m_House;
    vector<Player> m_Players;
};

Game::Game(const vector<string>& names)
{
    vector<string>::const_iterator pName;
    for (pName = names.begin(); pName != names.end(); ++pName)
    {
        m_Players.push_back(Player(*pName));
    }
    
    srand(static_cast<unsigned int>(time(0)));
    //m_Deck.Populate(); 
    //m_Deck.Shuffle();
}

Game::~Game()
{}
void Game::Play()
{
	m_Deck.Populate(); // removed from Game::Game
    m_Deck.Shuffle();  // removed from Game::Game
    vector<Player>::iterator pPlayer;
    for (int i = 0; i < 2; ++i)
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            m_Deck.Deal(*pPlayer); // polymorphism
        }
        m_Deck.Deal(m_House);  // polymorphism
    }
    
    m_House.FlipFirstCard();
    
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        cout << *pPlayer << endl;
    }
    cout << m_House << endl;
    
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        m_Deck.AdditionalCards(*pPlayer);  // polymorphism
    }
    
    m_House.FlipFirstCard();
    cout << endl << m_House;
    
    m_Deck.AdditionalCards(m_House);  // polymorphism 
    
    if (m_House.IsBoosted())
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            if (!(pPlayer->IsBoosted()))
            {
                pPlayer->Win();
            }
        }
    }
    else
    {
        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
        {
            if (!(pPlayer->IsBoosted()))
            {
                if (pPlayer->GetTotal() > m_House.GetTotal())
                {
                    pPlayer->Win();
                }
                else if (pPlayer->GetTotal() < m_House.GetTotal())
                {
                    pPlayer->Lose();
                }
                else
                {
                    pPlayer->Push();
                }
            }
        }
        
    }
    
    for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
    {
        pPlayer->Clear();
    }
    m_House.Clear();
}

//////////////////////////// main //////////////////////////////////

// Task5
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
    
    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> again;
    }



    return 0;
}

//Iterator: lista wszystkiego...pierwszy el, ostatni el, nastepny el, poprzedni el.... -> caly dzien :)

#include<iostream>
#define Table_Size 10
using namespace std;

template <typename K, typename V, typename F>
class Map;

template <typename K, typename V>
class MapNode;

template <typename K>
struct KeyHash;

//--------------------------------MAPA-----------------------------------------------------//

template <typename K, typename V>
class MapNode {

    // key-value pair
	K key; // traktuje go jako ID pracownika
    V value; // imie i naziwsko pracownika

    MapNode *next;

public:
    MapNode(const K &key, const V &value) :
    key(key), value(value), next(NULL) {
		//Shop_Things::workers_counter +=1;
    }

    template <typename U,typename G>
    friend ostream& operator<<(ostream & ob,const MapNode<U,G>& ex){
		ob<<"Key  ->"<<ex.key<<endl;
		ob<<"Value  ->"<<ex.value<<endl;

		}

	MapNode<K,V> operator =(MapNode<K,V> const &ob) {;
	key=ob.key;
	value=ob.value;

}

    K getKey() const {
        return key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V value) {
        this->value = value;
    }

    MapNode *getNext() const {
        return next;
    }

    void setNext(MapNode *next) {
        this->next = next;
    }

};


template <typename K>
struct KeyHash {
    unsigned long operator()(const K& key) const
    {
        return reinterpret_cast<unsigned long>(key) % Table_Size;
    }
};

struct MyKeyHash {
    unsigned long operator()(const int& k) const
    {
        return k % 10;
    }
};


template <typename K, typename V, typename F=KeyHash<K> >
class Map {

	MapNode<K, V> **table;
    F hashFunc;
public:

    friend class Itrator;

    Map() {

        table = new MapNode<K, V> *[Table_Size]();
    }

    ~Map() {

        for (int i = 0; i < Table_Size; ++i) {
            MapNode<K, V> *entry = table[i];
            while (entry != NULL) {
                MapNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = NULL;
        }
        // destroy the  table
        delete [] table;
    }





    bool get(const K &key, V &value) {
        unsigned long hashValue = hashFunc(key);
        MapNode<K, V> *entry = table[hashValue];

        while (entry != NULL) {
            if (entry->getKey() == key) {
                value = entry->getValue();
                return true;
            }
            entry = entry->getNext();
        }
        return false;
    }

    void put(const K &key, const V &value) {
        unsigned long hashValue = hashFunc(key);
        MapNode<K, V> *prev = NULL;
        MapNode<K, V> *entry = table[hashValue];
        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            entry = new MapNode<K, V>(key, value);
            if (prev == NULL) {
                // insert as first bucket
                table[hashValue] = entry;
            } else {
                prev->setNext(entry);
				//cout<<"Tutaj wk³adam wskaznik do NEXTA"<<endl;
				//cout<<"entry = "<<entry<<endl;
				//cout<<"**************************"<<endl;
            }
        } else {
            // just update the value
            entry->setValue(value);
        }
        //cout<<" Adres tej komorki to: "<<entry<<endl;
        //cout<<" Adres tej tablicy[0] to: "<<table[0]<<endl<<endl;
        //cout<<" Adres INDEKSU TABLICY: "<<table[hashValue]<<endl;
        //cout<<endl<<"- - - - - - - - - - - - - "<<endl;
    }

    void remove(const K &key) {
        unsigned long hashValue = hashFunc(key);
        MapNode<K, V> *prev = NULL;
        MapNode<K, V> *entry = table[hashValue];

        while (entry != NULL && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == NULL) {
            // key not found
            return;
        }
        else {
            if (prev == NULL) {
                // remove first bucket of the list
                table[hashValue] = entry->getNext();
            } else {
                prev->setNext(entry->getNext());
            }
            delete entry;
        }

            cout<<endl<<"- - - - - - - - - - - - - "<<endl<<endl;
    }



	void list(){
		MapNode<K,V> *ptr=NULL;
		MapNode<K,V> *next=NULL;
		//MapNode<K,V> *next;
		//cout<<"Poczatek funkcji -> ptr = "<<ptr<<endl;
		//cout<<"Poczatek funkcji -> table = "<<table[0]<<endl;
		cout<<"LISTA PRACOWNIKOW : "<<endl<<endl;
		int i=0,key;
		string value;
		int counter = 0 ;

		while(i<Table_Size){

			ptr=table[i];
			i++;

			if(ptr!=0){
			key=ptr->getKey();
			value=ptr->getValue();
			counter++;
			cout<<counter<<". pracownik to : "<<endl<<"		ID : "<<key<<endl<<"		Name : "<<value<<endl<<"---------------------------------------"<<endl;
			next=ptr->getNext();
				while(next!=0){
					key=next->getKey();
					value=next->getValue();
					counter++;
					cout<<counter<<". pracownik to : "<<endl<<"		ID : "<<key<<endl<<"		Name : "<<value<<endl<<"---------------------------------------"<<endl;


				}
			}
		}
    }



class Iterator{
	 MapNode<K,V> *current;

	public:

    Iterator(MapNode<K,V> *current):current(current){
		}


	MapNode<K,V>& operator*()const{
		return *current;
		}

	Iterator& operator++()const{
	    Map<K,V,F> hmap;
	    current=&hmap.next(*current);
	    return Iterator(current);
	    }

    bool operator==(const Iterator& ob) const {
        return current == ob.current;
    }


    bool operator!=(const Iterator& ob) const {
        return current != ob.current;
    }

	};





	Iterator begin(){
		MapNode<K,V> *ptr=NULL;
		int i=0;

		while(ptr==NULL){
			ptr=table[i];
			i++;
		}

		return Iterator(ptr);
    }


    Iterator end(){
		MapNode <K,V> *ptr=NULL;
		MapNode <K,V> *next=NULL;
		int i=Table_Size-1;

		while(ptr==NULL){
			ptr=table[i];
			i--;

			if(ptr!=NULL){
				//ptr=ptr->getNext();
				while(ptr->getNext()!=NULL) ptr=ptr->getNext();
			}


		}


		return Iterator(ptr);
	}





	Iterator next(MapNode <K,V> ob){                    //iter=hmap.next(*iter)
		MapNode <K,V> ptr(ob);
		MapNode <K,V> *wsk=NULL;

		int i=0;
		int licznik[2];//licznik[0]- index tablicy danego obiektu    licznik[1]-index tablicy nastepnego obiektu
		wsk=table[0];

		//cout<<" KEY 1 :"<<wsk->getKey()%10<<endl;
		//cout<<" KEY 2 :"<<ptr.getKey()%10<<endl;


		while(i<10)
        {  //Tutaj sprawdzam w ktorej komorce tablicy lezy iterator

            if(wsk!=NULL)
            {

                if(wsk->getKey()%10==ptr.getKey()%10)
                { //szukam indeksu tablicy i przypisuje go do licznik[0]
                    licznik[0]=i;
                    int j=licznik[0]+1;
                    wsk=table[j];

                    while(j<10)
                    { //tutaj szukam indexu kolejnego elementu i przypisuje go do licznik[1]

                        if(wsk!=NULL)
                        {
                            licznik[1]=j;
                            break;
                        }

                    j++;
                    wsk=table[j];
                    }
                }
			}
			i++;
			wsk=table[i];
		}


	//cout<<"Licznik 1 : "<<licznik[0]<<endl;
	//cout<<"Licznik 2 : "<<licznik[1]<<endl;

    /*w tym miejscu spradzam czy wykorzystana jest lista w danym indeksie tablicy,
    jesli jest ide po kolejnych jej elementach jesli nie przechodze do nastepnego indeksu tablicy (lista[1])*/
	wsk=table[licznik[0]];


	if(ptr.getNext()!=NULL){
        wsk=&ptr;

		while(wsk->getNext()!=NULL){
            wsk=wsk->getNext();
            break;
		}

	}//wsk=wsk->getNext();
	else wsk=table[licznik[1]];


	return Iterator(wsk);
	}


};//KONIEC MAPY










int main(){
Map<int, string, MyKeyHash> hmap;
Map<int, string, MyKeyHash> test;

	hmap.put(96,"asdl");
	hmap.put(20,"asdil");
	//hmap.put(16,"sasail");
	//hmap.put(86,"sasail");
	//hmap.put(58,"Kamil");
	hmap.put(10,"PIOTREK");
	hmap.put(30,"Kamil");
	//hmap.put(90,"Kamil");
	//hmap.list();

Map<int, string, MyKeyHash>::Iterator iter=hmap.begin();
Map<int, string, MyKeyHash>::Iterator iter2=hmap.begin();
//iter=hmap.end();

MapNode<int,string> ob(*iter);
cout<<" KEY : "<<ob.getKey()<<endl;



cout<<"Wskaznik jeden ->   "<<*iter<<endl;

iter=hmap.next(*iter);

cout<<"Wskaznik dwa  ->   "<<*iter<<endl;

iter=hmap.next(*iter);

cout<<"Wskaznik trzy ->   "<<*iter<<endl;

iter=hmap.next(*iter);

cout<<"Wskaznik cztery ->   "<<*iter<<endl;

cout<<"DZIEKUJE ZA WSPANIALA WSPOLPRACE :)"<<endl;

if(iter2!=iter) cout<<"PRAWDA"<<endl;
else cout<<"FaUSZ"<<endl;


cout<<"-----------------------------------------------------------"<<endl;

/*for(iter=hmap.begin();iter=hmap.end();iter=hmap.next){

    cout<<*iter<<endl;
}*/


return 0;
}

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int NumBooks,NumLibraries,Days;
int scores[110000];

class Library;

class Book{
public:
    int id;
    int score;
    int bn=0;
    vector <Library*> belongs_to;
}zero;
bool operator<(Book a, Book b)
{
	return a.score < b.score;
}

class Library{
public:
    int id;
    int num_books;
    int signup;
    int book_sum=0;
    int books_per_day;
    int diff_num=0;
    vector <Book*> books;
    friend bool operator < (const Library &a, const Library &b) {
        return  a.diff_num/a.num_books > b.diff_num/b.num_books;
    }
 };
inline bool in_library_compare(const Book *a, const Book *b) {
    return a->score > b->score;
}

Library libraries[30001];
Book books[100001];


int mark[100005],marklib[100005];
int Max_Puan(std::vector<Library> order,int print=0,int k1=10,int k2=10,int k3=10)
{
	order.clear();

	int sum=0;
	vector <priority_queue <Book>  > pqs;
	priority_queue <Book> empty;
	int next_sing_day=0,library_cnt=0;

	std::vector<int> print_lib; // print funcs
	vector < set <int>  > print_books; // print funcs
	set <int> emp;
	memset(mark,0,sizeof mark);
	memset(marklib,0,sizeof marklib);
	for (int day = 0; day < Days; ++day)
	{
//		cerr << next_sing_day << endl;
		if(day==next_sing_day)
		{
			if(day>0)
			{
				print_lib.push_back(order[library_cnt].id);
				pqs.push_back(empty);
				print_books.push_back(emp);

				for (auto it : order[library_cnt].books)
				{
					pqs[library_cnt].push(*it);
				}
				library_cnt++;
			}
			long long int bestl=-10000000000LL,bestid=-1;
			for (int i = 0; i < NumLibraries; ++i)
			{
				if(marklib[i]==1) continue;
				if(day==0)
					sort(libraries[i].books.begin(),libraries[i].books.end(),in_library_compare);

				int canget=(Days-libraries[i].signup)*libraries[i].books_per_day;
				int top= 0;
				for (auto it : libraries[i].books)
				{
					if(mark[it->id]==0)
					{
						canget--;
						top+=it->score;
					}
					if(canget==0) break;
				}
				top/=pow(libraries[i].signup,1.3);
//				top-=20.1*libraries[i].diff_num/libraries[i].num_books;
				if(top>bestl)
				{
					bestl=top;
					bestid=i;
				}
			}
			marklib[bestid]=1;
			if(bestid==-1)
			{
				bestid=0;
				next_sing_day=Days;
			}
			order.push_back(libraries[bestid]);
			next_sing_day+=order[library_cnt].signup;
		}
		std::vector<int> add;
		add.clear();
		for (int i = 0; i < library_cnt; ++i)
			for (int j = 0; j < order[i].books_per_day; ++j)
			{
				if(pqs[i].empty()) break;
				add.push_back(i);
			}
		for (int ii = 0; ii < add.size(); ++ii)
		{
			int i=add[ii];
			if(pqs[i].empty()) continue;
			
			Book tmp = pqs[i].top();
			pqs[i].pop();
			
			if(mark[tmp.id])
			{
				int old = mark[tmp.id] - 1;	
				if((pqs[old].empty() ?  0 : pqs[old].top().score ) < (pqs[i].empty() ?  0 : pqs[i].top().score))
				{
					ii--;
					continue;
				}
				else
				{
					if(print==1)
					{
						print_books[old].erase(tmp.id);
						print_books[i].insert(tmp.id);
					}
					mark[tmp.id]=i+1;
					add.push_back(old);
				}
				continue;
			}
			if(print==1)
			{
				print_books[i].insert(tmp.id);
			}
			sum+=tmp.score;
			mark[tmp.id]=i+1;
		}
	}
	if(print==1)
	{
		printf("%d\n",(int)print_lib.size());
		for (int i = 0; i < (int)print_lib.size(); ++i)
		{
			if(print_books[i].size()==0)
				print_books[i].insert(libraries[print_lib[i]].books[0]->id );
			printf("%d %d\n",print_lib[i],(int)print_books[i].size());
			int fl=0;


			for (auto it : print_books[i])
			{
				fl=1;
				printf("%d ",it);
			}
			if(fl==1)
			printf("\n");
		}
		cerr << sum << endl;
		exit(0);
	}
	return sum;
}

int main(){
    
    cin >> NumBooks >> NumLibraries >> Days;
    
    for( int i=0 ; i<NumBooks ; i++ ) {
        scanf(" %d",&scores[i]);
        books[i].score = scores[i];
        books[i].id = i;
    }
    for( int i=0 ; i<NumLibraries ; i++ ){
        libraries[i].id = i;
        scanf(" %d",&libraries[i].num_books);
        scanf(" %d",&libraries[i].signup);
        scanf(" %d",&libraries[i].books_per_day);
        for( int j=0,dummy ; j<libraries[i].num_books ; j++ ){
            scanf(" %d",&dummy);
            libraries[i].books.push_back(&books[dummy]);
            books[dummy].belongs_to.push_back(&libraries[i]);
            books[dummy].bn+=1;
            libraries[i].book_sum += books[dummy].score;
        }
    }
    for( int i=0 ; i<NumLibraries ; i++ )
        for( int j=0 ; j<libraries[i].num_books ; j++ )
			libraries[i].diff_num+=libraries[i].books[j]->bn;    	
	sort(libraries, libraries + NumLibraries);

	std::vector<Library> order;    

  	Max_Puan(order,1);

    return 0;
}
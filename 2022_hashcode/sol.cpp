#include <bits/stdc++.h>
#define INPUT_FILE_NUM 6
#define THREAD_COUNT 5
#define inf 1000000000
using namespace std;
pthread_mutex_t mutexLock; //   pthread_mutex_lock( & mutexLock);  pthread_mutex_unlock( & mutexLock);



const string FILE_NAMES[6] = {"a_an_example.in.txt", "b_better_start_small.in.txt",
    "c_collaboration.in.txt",
    "d_dense_schedule.in.txt",
    "e_exceptional_skills.in.txt",
    "f_find_great_mentors.in.txt"};
/*
void readFile(string fileName) {
    ifstream cin(fileName);
    int a;
    cin >> a;
}
void writeFile( solution, string fileName, string folderName = "solution/") {
    ofstream fout(folderName + "/sol_" + fileName);
    fout << 0 << endl;
//    printf("writeFile Done : %s\n", fileName.c_str());
}
*/





class Contributor {
    public:
    unordered_map<int,int> skillLevel; //  skill id - level id
    int skill_count;
    int last_time = 0;
    Contributor() {
        last_time = 0;
    }
    int getSkill(int id) {
        if(skillLevel.count(id))
            return skillLevel[id];
        return 0;
    }
};
class Project {
    public:
    vector<pair<int,int> > roles; //  skill id - level id
    vector<int> current_people;
    int duration, score, bestday, role_count;   
    int done;
};
class Solver
{
    public:
    string fileName;
    unordered_map <string, int> MAP_name_id,MAP_skill_id,MAP_project_id;
    unordered_map <int, string> MAP_id_name,MAP_id_skill,MAP_id_project;
    int lastSkillId, lastProjectId;
    int contributor_count, project_count;
    vector<Contributor> contributors;
    vector<Project> projects;
    vector<int> current_order;
    vector<int> mark;
    queue<int> not_used;
    
    Solver(string _FILE_NAME) {        
        lastSkillId = 0;
        lastProjectId = 0;
        fileName = _FILE_NAME;
    }

    int getSkillId(string skill) {
        if(MAP_skill_id[skill])
            return MAP_skill_id[skill];
        MAP_skill_id[skill] = ++lastSkillId;
        MAP_id_skill[lastSkillId] = skill;
        return lastSkillId;
    }
    void read_file() {
        cout << "START READ" << endl;
        ifstream cin(fileName);
        cin >> contributor_count >> project_count;
        mark.resize(contributor_count, 0);
        for (int i = 0; i < contributor_count; i++)
        {   
            Contributor c;
            string name;            
            cin >> name >> c.skill_count;
            MAP_name_id[name] = i;
            MAP_id_name[i] = name;

            for (int i = 0; i < c.skill_count; i++) {
                string skill_name;
                int skill_level;
                cin >> skill_name >> skill_level;
                c.skillLevel[getSkillId(skill_name)] = skill_level;
            }
            contributors.push_back(c);
        }
        for (int i = 0; i < project_count; i++)
        {
            string name;
            Project p;
            cin >> name >> p.duration >> p.score >> p.bestday >> p.role_count;
            MAP_project_id[name] = i;
            MAP_id_project[i] = name;
            for (int i = 0; i < p.role_count; i++) {
                string skill_name;
                int skill_level;
                cin >> skill_name >> skill_level;
                p.roles.push_back({getSkillId(skill_name), skill_level});
            }
            projects.push_back(p);
        }
    }
    bool cmp(int a, int b) {
        return projects[a].bestday<projects[b].bestday;
    }
            vector <int> curr_roles;
            int finish_time,project_point;
    int put_point(int id){ 
        project_point = 0;
        finish_time= 0; 
        projects[id].done = 1;
            int begin_time = 0;
            for(pair<int,int> skill : projects[id].roles) {
                int best_score = 0, best_id = 0; 
                for (int i = 0; i < contributor_count; i++)
                    if(mark[i] == 0) {
                        if(contributors[i].getSkill(skill.first) >= skill.second) {
                            int this_score = inf - contributors[i].last_time;
                            if(this_score > best_score) {
                                best_id = i;
                                best_score = this_score;
                            }
                        }
                        if(contributors[i].getSkill(skill.first) == skill.second) {
                            int this_score = inf - contributors[i].last_time +10;
                            if(this_score > best_score) {
                                best_id = i;
                                best_score = this_score;
                            }
                        }
                    }
                if(best_score == 0) {
                    projects[id].done = 0;
                    break;
                }
                begin_time = max(begin_time, contributors[best_id].last_time);
                curr_roles.push_back(best_id);
                mark[best_id] = 1;
            }
            for(int i : curr_roles) {
                mark[i] = 0;
            }
            finish_time = begin_time + projects[id].duration;
            project_point = max(0, projects[id].score + min(projects[id].bestday-finish_time, 0));
            if(project_point == 0)
                projects[id].done = 0;
            if(projects[id].done == 0)
                return 0;
            return project_point;
    }
    int fill() {
        cout << "START FILL" << endl;
        current_order.clear();
        vector<int >some_ord;

        for (int i = 0; i < project_count; i++)
            some_ord.push_back(i);

        
        sort(some_ord.begin(), some_ord.end(), [&](int& a ,int& b) {
         return projects[a].bestday < projects[b].bestday;
});

        for (int i = 0; i < project_count; i++)
            not_used.push(some_ord[i]);


        int total_point = 0;
        int debug = 0;
        int tr = 0;
        while(not_used.empty() == false)
        {
            if(tr >= 10000)
                break;
            curr_roles.clear();
            int id = not_used.front();
            not_used.pop();
            project_point = put_point(id);
            if(project_point <= 4) {
                not_used.push(id);
                tr+=1;
                continue;
            }
            for(int i = 0; i < curr_roles.size(); i++) {
                contributors[curr_roles[i]].last_time = finish_time;
                if(contributors[curr_roles[i]].getSkill(projects[id].roles[i].first) == projects[id].roles[i].second)
                    contributors[curr_roles[i]].skillLevel[projects[id].roles[i].first] += 1;
            }
            debug = max(debug,finish_time);
            projects[id].current_people = curr_roles;
            assert(projects[id].score>0);
            assert(projects[id].score<= 100000);
            total_point += project_point;
            current_order.push_back(id);
            assert(project_point > 0);
//            cout << total_point << endl;
        }
        return total_point;
    }
    void printSolution(string folderName = "solution/") {
        ofstream fout(folderName + "/sol_" + fileName);
        int projects_planned = 0;
        for (int id : current_order)
            if(projects[id].done == 1)
                projects_planned += 1;
        fout << projects_planned << endl;
        for (int id : current_order)
            if(projects[id].done == 1)
            {
                fout << MAP_id_project[id] << endl;
                for (auto it : projects[id].current_people)
                {
                    fout << MAP_id_name[it] << " ";
                }
                fout << endl;
            }
    }
/*
    int calc_score() {
        int score = 0;
        for(auto project : current_order) {
            int begin_time = 0;
            for (int i = 0; i < project.role_count; i++) {
                begin_time = max(begin_time, last_times[project.current_people[i]]);
            }
            int finish_time = begin_time + project.duration;
            int project_point = max(0, project.score - min(project.bestday-finish_time, 0));
            score += project_point;
            for (int i = 0; i < project.role_count; i++) {
                last_times[project.current_people[i]] = finish_time;
            }
        }
        return score;
    }*/
};




void solve(string fileName) { 
    Solver s(fileName);
    s.read_file();
    cout << "Total_point:" << s.fill() << endl;
    s.printSolution();
}

int32_t main(int32_t argc, char** argv) {
    srand(time(NULL));
    if (argc == 1) { // NO ARGUMENTS, RUN ALL
        vector<thread> threads;        
        for (int i = 0; i < INPUT_FILE_NUM; i++)
            threads.push_back(thread(solve,FILE_NAMES[i]));
        for (auto &th : threads)
            th.join();
    } else {
        int c = argv[1][0] >= 'a' ? argv[1][0] - 'a' :  ((argv[1][0] < 'A') ? argv[1][0] - '1' : argv[1][0] - 'A');
        solve(FILE_NAMES[c]);    
    }
}
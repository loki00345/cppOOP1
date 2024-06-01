#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> 

using namespace std;

class Student {
    string name;
    int id;
    int age;
    string major;

public:
    Student() : name(""), id(0), age(0), major("") {}

    Student(const string& name, int id, int age, const string& major) : name(name), id(id), age(age), major(major) {}

    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    int getId() const { return id; }
    void setId(int id) { this->id = id; }

    int getAge() const { return age; }
    void setAge(int age) { this->age = age; }

    string getMajor() const { return major; }
    void setMajor(const string& major) { this->major = major; }

    void print() const {
        cout << "ID: " << id << "\nName: " << name << "\nAge: " << age << "\nMajor: " << major << endl;
    }

    void writeFile(ofstream& fout) const {
        fout << id << '\n' << name << '\n' << age << '\n' << major << '\n';
    }

    void readFile(ifstream& fin) {
        fin >> id;
        fin.ignore();
        getline(fin, name);
        fin >> age;
        fin.ignore();
        getline(fin, major);
    }
};

class StudentDatabase {
    Student* students;
    int capacity;
    int count;

    void resize() {
        capacity *= 2;
        Student* newStudents = new Student[capacity];
        for (int i = 0; i < count; ++i) {
            newStudents[i] = students[i];
        }
        delete[] students;
        students = newStudents;
    }

public:
    StudentDatabase() : capacity(10), count(0) {
        students = new Student[capacity];
    }

    ~StudentDatabase() {
        for (size_t i = 0; i < count; i++)
        {
            delete students[i];
        }
        
    }

    int GetCount() const {
        return count;
    }

    void addStudent(const Student& s) {
        if (count == capacity) {
            resize();
        }
        students[count++] = s;
    }

    void removeStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i].getId() == id) {
                for (int j = i; j < count - 1; ++j) {
                    students[j] = students[j + 1];
                }
                --count;
                cout << "Student with ID " << id << " deleted." << endl;
                return;
            }
        }
        cout << "Student with ID " << id << " not found.\n" << endl;
    }

    Student* findStudent(int id) const {
        for (int i = 0; i < count; ++i) {
            if (students[i].getId() == id) {
                return &students[i];
            }
        }
        return nullptr;
    }

    void listStudents() const {
        for (int i = 0; i < count; ++i) {
            students[i].print();
            cout << "--------------------" << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Error saving file\n" << endl;
            return;
        }
        else {
            cout << "Saved!\n" << endl;
        }
        fout << count << endl;
        for (int i = 0; i < count; ++i) {
            students[i].writeFile(fout);
        }
        fout.close();
    }

    void loadFromFile(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Error opening file\n" << endl;
            return;
        }
        else {
            cout << "Loaded!\n" << endl;
        }
        int fileCount;
        fin >> fileCount;
        fin.ignore();

        if (fileCount > capacity) {
            delete[] students;
            capacity = fileCount;
            students = new Student[capacity];
        }

        count = 0;
        for (int i = 0; i < fileCount; ++i) {
            Student temp;
            temp.readFile(fin);
            addStudent(temp);
        }

        fin.close();
    }

    void updateStudent(int id, const Student& data) {
        Student* student = findStudent(id);
        if (student) {
            *student = data;
            cout << "Student with ID " << id << " updated." << endl;
        }
        else {
            cout << "Student with ID " << id << " not found." << endl;
        }
    }

    void filterByMajor(const string& major) const {
        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (students[i].getMajor() == major) {
                students[i].print();
                cout << "--------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Not found\n" << endl;
        }
    }
    void filterByAge(int minAge, int maxAge) const {
        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (students[i].getAge() >= minAge && students[i].getAge() <= maxAge) {
                students[i].print();
                cout << "--------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No students found within the given age range." << endl;
        }
    }

    bool compId(const Student& a, const Student& b)
    {
        return a.getId() < b.getId();
    }

    void sortStudents(const string& criterion) {
        if (criterion == "name") {
            sort(students, students + count, [](const Student& a, const Student& b) {
                return a.getName() < b.getName();
                });
        }
        else if (criterion == "id") {
            sort(students, students + count, compId);
        }
        else if (criterion == "age") {
            sort(students, students + count, [](const Student& a, const Student& b) {
                return a.getAge() < b.getAge();
                });
        }
        else {
            cout << "Invalid criterion for sorting." << endl;
        }
    }



};

void menu() {
    StudentDatabase studentDB;
    int choice;
    string filename = "students.txt";
    do {
        cout << "1. Add student\n2. Delete student\n3. Search student\n4. List all students\n5. Save to file\n6. Load from file\n7. Update student\n8. Filter by major\n9. Filter by age\n10. Count students\n11. Sort students\n0. Exit\n\nEnter your choice: ";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1: {
            string name, major;
            int id, age;
            cout << "Enter student ID: "; cin >> id;
            cin.ignore();
            cout << "Enter student name: "; getline(cin, name);
            cout << "Enter student age: "; cin >> age;
            cin.ignore();
            cout << "Enter student major: "; getline(cin, major);
            studentDB.addStudent(Student(name, id, age, major));
            cout << endl;
            break;
        }
        case 2: {
            int id;
            cout << "Enter student ID to delete: "; cin >> id;
            studentDB.removeStudent(id);
            cout << endl;
            break;
        }
        case 3: {
            int id;
            cout << "Enter student ID to search: "; cin >> id;
            Student* student = studentDB.findStudent(id);
            if (student) {
                student->print();
            }
            else {
                cout << "Student with ID " << id << " not found.\n" << endl;
            }
            cout << endl;
            break;
        }
        case 4:
            studentDB.listStudents();
            cout << endl;
            break;
        case 5:
            studentDB.saveToFile(filename);
            break;
        case 6:
            studentDB.loadFromFile(filename);
            break;
        case 7: {
            int id, age;
            string name, major;
            cout << "Enter student ID to update: "; cin >> id; cin.ignore();
            cout << "Enter new student name: "; getline(cin, name);
            cout << "Enter new student age: "; cin >> age; cin.ignore();
            cout << "Enter new student major: "; getline(cin, major);
            studentDB.updateStudent(id, Student(name, id, age, major));
            cout << endl;
            break;
        }
        case 8: {
            string major;
            cout << "Enter major to filter by: "; cin.ignore(); getline(cin, major);
            studentDB.filterByMajor(major);
            cout << endl;
            break;
        }
        case 9: {
            int min, max;
            cout << "Enter min age: "; cin >> min; cout << endl;
            cout << "Enter max age: "; cin >> max; cout << endl;
            studentDB.filterByAge(min, max);
            cout << endl;
            break;
        }
        case 10: {
            cout << "Number of students: " << studentDB.GetCount() << "\n\n";
            break;
        }
        case 11: {
            string criteria;
            cout << "Enter criteria: " << endl; cin >> criteria;
            studentDB.sortStudents(criteria);
            break;
        }
        /*case 0:
            cout << "Exiting...\n";
            break;*/
        default:
            cout << "Invalid choice! Please try again.\n\n";
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> 

using namespace std;

class Student {
    string name;
    int id;
    int age;
    string major;

public:
    Student() : name(""), id(0), age(0), major("") {}

    Student(const string& name, int id, int age, const string& major) : name(name), id(id), age(age), major(major) {}

    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    int getId() const { return id; }
    void setId(int id) { this->id = id; }

    int getAge() const { return age; }
    void setAge(int age) { this->age = age; }

    string getMajor() const { return major; }
    void setMajor(const string& major) { this->major = major; }

    void print() const {
        cout << "ID: " << id << "\nName: " << name << "\nAge: " << age << "\nMajor: " << major << endl;
    }

    void writeFile(ofstream& fout) const {
        fout << id << '\n' << name << '\n' << age << '\n' << major << '\n';
    }

    void readFile(ifstream& fin) {
        fin >> id;
        fin.ignore();
        getline(fin, name);
        fin >> age;
        fin.ignore();
        getline(fin, major);
    }
};

class StudentDatabase {
    Student* students;
    int capacity;
    int count;

    void resize() {
        capacity *= 2;
        Student* newStudents = new Student[capacity];
        for (int i = 0; i < count; ++i) {
            newStudents[i] = students[i];
        }
        delete[] students;
        students = newStudents;
    }

public:
    StudentDatabase() : capacity(10), count(0) {
        students = new Student[capacity];
    }

    ~StudentDatabase() {
        for (size_t i = 0; i < count; i++)
        {
            delete students[i];
        }
        
    }

    int GetCount() const {
        return count;
    }

    void addStudent(const Student& s) {
        if (count == capacity) {
            resize();
        }
        students[count++] = s;
    }

    void removeStudent(int id) {
        for (int i = 0; i < count; ++i) {
            if (students[i].getId() == id) {
                for (int j = i; j < count - 1; ++j) {
                    students[j] = students[j + 1];
                }
                --count;
                cout << "Student with ID " << id << " deleted." << endl;
                return;
            }
        }
        cout << "Student with ID " << id << " not found.\n" << endl;
    }

    Student* findStudent(int id) const {
        for (int i = 0; i < count; ++i) {
            if (students[i].getId() == id) {
                return &students[i];
            }
        }
        return nullptr;
    }

    void listStudents() const {
        for (int i = 0; i < count; ++i) {
            students[i].print();
            cout << "--------------------" << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Error saving file\n" << endl;
            return;
        }
        else {
            cout << "Saved!\n" << endl;
        }
        fout << count << endl;
        for (int i = 0; i < count; ++i) {
            students[i].writeFile(fout);
        }
        fout.close();
    }

    void loadFromFile(const string& filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Error opening file\n" << endl;
            return;
        }
        else {
            cout << "Loaded!\n" << endl;
        }
        int fileCount;
        fin >> fileCount;
        fin.ignore();

        if (fileCount > capacity) {
            delete[] students;
            capacity = fileCount;
            students = new Student[capacity];
        }

        count = 0;
        for (int i = 0; i < fileCount; ++i) {
            Student temp;
            temp.readFile(fin);
            addStudent(temp);
        }

        fin.close();
    }

    void updateStudent(int id, const Student& data) {
        Student* student = findStudent(id);
        if (student) {
            *student = data;
            cout << "Student with ID " << id << " updated." << endl;
        }
        else {
            cout << "Student with ID " << id << " not found." << endl;
        }
    }

    void filterByMajor(const string& major) const {
        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (students[i].getMajor() == major) {
                students[i].print();
                cout << "--------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Not found\n" << endl;
        }
    }
    void filterByAge(int minAge, int maxAge) const {
        bool found = false;
        for (int i = 0; i < count; ++i) {
            if (students[i].getAge() >= minAge && students[i].getAge() <= maxAge) {
                students[i].print();
                cout << "--------------------" << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No students found within the given age range." << endl;
        }
    }

    bool compId(const Student& a, const Student& b)
    {
        return a.getId() < b.getId();
    }

    void sortStudents(const string& criterion) {
        if (criterion == "name") {
            sort(students, students + count, [](const Student& a, const Student& b) {
                return a.getName() < b.getName();
                });
        }
        else if (criterion == "id") {
            sort(students, students + count, compId);
        }
        else if (criterion == "age") {
            sort(students, students + count, [](const Student& a, const Student& b) {
                return a.getAge() < b.getAge();
                });
        }
        else {
            cout << "Invalid criterion for sorting." << endl;
        }
    }



};

void menu() {
    StudentDatabase studentDB;
    int choice;
    string filename = "students.txt";
    do {
        cout << "1. Add student\n2. Delete student\n3. Search student\n4. List all students\n5. Save to file\n6. Load from file\n7. Update student\n8. Filter by major\n9. Filter by age\n10. Count students\n11. Sort students\n0. Exit\n\nEnter your choice: ";
        cin >> choice;
        cout << endl;
        switch (choice) {
        case 1: {
            string name, major;
            int id, age;
            cout << "Enter student ID: "; cin >> id;
            cin.ignore();
            cout << "Enter student name: "; getline(cin, name);
            cout << "Enter student age: "; cin >> age;
            cin.ignore();
            cout << "Enter student major: "; getline(cin, major);
            studentDB.addStudent(Student(name, id, age, major));
            cout << endl;
            break;
        }
        case 2: {
            int id;
            cout << "Enter student ID to delete: "; cin >> id;
            studentDB.removeStudent(id);
            cout << endl;
            break;
        }
        case 3: {
            int id;
            cout << "Enter student ID to search: "; cin >> id;
            Student* student = studentDB.findStudent(id);
            if (student) {
                student->print();
            }
            else {
                cout << "Student with ID " << id << " not found.\n" << endl;
            }
            cout << endl;
            break;
        }
        case 4:
            studentDB.listStudents();
            cout << endl;
            break;
        case 5:
            studentDB.saveToFile(filename);
            break;
        case 6:
            studentDB.loadFromFile(filename);
            break;
        case 7: {
            int id, age;
            string name, major;
            cout << "Enter student ID to update: "; cin >> id; cin.ignore();
            cout << "Enter new student name: "; getline(cin, name);
            cout << "Enter new student age: "; cin >> age; cin.ignore();
            cout << "Enter new student major: "; getline(cin, major);
            studentDB.updateStudent(id, Student(name, id, age, major));
            cout << endl;
            break;
        }
        case 8: {
            string major;
            cout << "Enter major to filter by: "; cin.ignore(); getline(cin, major);
            studentDB.filterByMajor(major);
            cout << endl;
            break;
        }
        case 9: {
            int min, max;
            cout << "Enter min age: "; cin >> min; cout << endl;
            cout << "Enter max age: "; cin >> max; cout << endl;
            studentDB.filterByAge(min, max);
            cout << endl;
            break;
        }
        case 10: {
            cout << "Number of students: " << studentDB.GetCount() << "\n\n";
            break;
        }
        case 11: {
            string criteria;
            cout << "Enter criteria: " << endl; cin >> criteria;
            studentDB.sortStudents(criteria);
            break;
        }
        /*case 0:
            cout << "Exiting...\n";
            break;*/
        default:
            cout << "Invalid choice! Please try again.\n\n";
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}

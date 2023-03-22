#include<iostream>
#include<cmath>
#include<string>
#include<unistd.h>

using namespace std;


class Room
{
public:
    Room();
    Room(float semiMinorAxis, float semiMajorAxis) : semiMajorAxis(semiMajorAxis), semiMinorAxis(semiMinorAxis)
    {
        eccentricity = sqrt(1 - pow((semiMinorAxis/semiMajorAxis), 2));
    }

    void SetMushroom(float distanceBtwnHalfs, float stalkWidth, float stalkHeight) {
        Room::distanceBtwnHalfs = distanceBtwnHalfs;
        Room::stalkHeight = stalkHeight; 
        Room::stalkWidth = stalkWidth;
    }
    string GetMushroom(){}
    ~Room();
private:
    float semiMinorAxis; 
    float semiMajorAxis;
    
    float distanceBtwnHalfs;
    float stalkWidth;
    float stalkHeight;

    float dotPosition[2];
    float eccentricity;
};

class Ray
{
public:
    // CrossPosition(){};
private:
    float inseptionPosition[2];
    float angle;
};

class Cell{
public:
    void SetDelta(float delta){
        Cell::delta = delta;
    }
    float GetDelta(){
        return delta;
    }
    void SetVelocity(float velocity){
        Cell::velocity = velocity;
    }
    float GetVelocity(){
        return velocity;
    }
private:
    float delta = 0;
    float velocity = 0;
    int pos[2];
};


class Space{
public:
    Space(int size){
        SetSize(size);
    }
    void SetSize(int size){
        Space::size = size;
    }
    int GetSize(){
        return(Space::size);
    }
    // virtual float GetDelta(int pos) = 0;
    // virtual float GetVelocity(int pos) = 0;
    virtual void Refresh() = 0;
    
    ~Space(){
        delete [] space;
    }
protected:
    int size;
    Cell *space;
};

class Space1 : public Space
{
public:
    Space1(int size) : Space(size) {
        space = new Cell[size];
        for(int i = 0; i < size; i++){
            space[i].SetDelta(0);
            // cout << i << " = " << space[i].GetDelta() << endl;
        }
    };
    Cell* GetSpace(){
        return space;
    }
    float GetDelta(int pos){
        float delta, velocity;
        velocity = space[pos].GetVelocity();
        delta = space[pos].GetDelta() + velocity/100;
        return delta;
    }
    float GetVelocity(int pos){
        float delta, velocity;
        // if (pos == 0){
        //     delta = (space[pos + 1].GetDelta() + space[pos].GetDelta()) / 2 - space[pos].GetDelta();
        // }
        // else if (pos == size - 1){
        //     delta = (space[pos].GetDelta() + space[pos - 1].GetDelta()) / 2 - space[pos].GetDelta();
        // }
        // else {
        //     delta = (space[pos + 1].GetDelta() 
        //                 + space[pos > 0 ? pos - 1 : 0].GetDelta()) / 2 
        //                 - space[pos < size - 1 ? pos + 1 : size - 1].GetDelta();
        // }
        delta = (space[pos + 1].GetDelta() 
                        + space[pos > 0 ? pos - 1 : 0].GetDelta()) / 2 
                        - space[pos < size - 1 ? pos + 1 : size - 1].GetDelta();
        velocity = space[pos].GetVelocity();
        velocity = velocity + delta/1000;
        return velocity;
    }
    void Refresh(){
        Cell *nextSpace = new Cell [size];
        for(int i = 0; i < size; i++){
            float delta, velocity;
            int pos = i;
            delta = GetDelta(pos);
            velocity = GetVelocity(pos);
            nextSpace[i].SetDelta(delta);
            nextSpace[i].SetVelocity(velocity);
        }
        for(int i = 0; i < size; i++){
            space[i] = nextSpace[i];
        }
        delete [] nextSpace;
    }
private:
    int pos[2];
};

class Space2 : public Space
{
public:
    Space2(int size) : Space(size) {
        space = new Cell* [size];
        for (int i = 0; i < size; i++){
            space[i] = new Cell [size];
        }
    }
    Cell** GetSpace(){
        return space;
    }
    float GetDelta(int pos[2]){
        float delta, velocity;
        velocity = space[pos[0]][pos[1]].GetVelocity();
        delta = space[pos[0]][pos[1]].GetDelta() + velocity/100;
        return delta;
    }
    float GetVelocity(int pos[2]){
        float delta, velocity;
        delta = (space[pos[0] < size - 1 ? pos[0] + 1 : size - 1][pos[1]].GetDelta() 
                    + space[pos[0] > 0 ? pos[0] - 1 : 0][pos[1]].GetDelta() 
                    + space[pos[0]][pos[1] < size - 1 ? pos[1] - 1 : size - 1].GetDelta() 
                    + space[pos[0]][pos[1] > 0 ? pos[1] - 1 : 0].GetDelta()) / 4 
                    - space[pos[0]][pos[1]].GetDelta();
        velocity = space[pos[0]][pos[1]].GetVelocity();
        velocity = velocity + delta/1000;
        return velocity;
    }
    void Refresh(){
        // Cell **nextSpace = new Cell* [size];
        // for (int i = 0; i < size; i++){
        //     nextSpace[i] = new Cell [size];
        // }
        // for(int i = 0; i < size; i++){
        //     for(int j = 0; j < size; j++){
        //         float delta, velocity;
        //         int *pos = new int[2];
        //         pos[0] = i;
        //         pos[1] = j;
        //         delta = GetDelta(pos);
        //         velocity = GetVelocity(pos);
        //         // nextSpace[i][j].SetDelta(delta);
        //         // nextSpace[i][j].SetVelocity(velocity);

        //         space[i][j].SetDelta(delta);
        //         space[i][j].SetVelocity(velocity);

        //         delete [] pos; 
        //     }
        // }
        for(int i = 0; i < size * size - 1; i++){
            float delta, velocity;
            int *pos = new int[2];
            pos[0] = i / size;
            pos[1] = i % size;
            delta = GetDelta(pos);
            velocity = GetVelocity(pos);
            // nextSpace[i][j].SetDelta(delta);
            // nextSpace[i][j].SetVelocity(velocity);

            space[pos[0]][pos[1]].SetDelta(delta);
            space[pos[0]][pos[1]].SetVelocity(velocity);

            delete [] pos; 
        }
        // for(int i = 0; i < size; i++){
        //     for(int j = 0; j < size; j++){
        //         space[i][j] = nextSpace[i][j];
        //     }
        // }
        // for (int i = 0; i < 2; i++){
        //     delete [] nextSpace[size];
        // }
    }
private:
    // int pos[2];
    Cell **space;
};

int main(void)
{
    // Space1 *space = new Space1(1000000);
    // space->GetSpace()[0].SetDelta(1);

    Space2 *space = new Space2(1000);
    space->GetSpace()[0][0].SetDelta(1);
    
    int j = 0;
    while(1){
        space->Refresh();
        if(j % 100 == 0){
            // for(int i = 0; i < space->GetSize(); i++){
            //     for(int j = 0; j < space->GetSize(); j++){
            //         // cout << space->GetSpace()[i][j].GetDelta() << " ";
            //     }
            //     // cout << endl;
            // }
            cout << j;
            cout << endl;
            cout << endl;
        }
        // usleep(10000);
        j++;
    }
    // задать класс комнаты с наследование поведения гриба, по умолчанию должен быть стандартный круглый гриб
    // комната должна последовательно содержать информацию об образующем эллипсе и о ножке гриба
    // применить более точный численный метод (напр. Рунге-Кутты)
    return 0;
}

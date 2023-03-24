#include<iostream>
#include<cmath>
#include<string>
#include<unistd.h>
#include<fstream>

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
    void SetMass(float mass){
        Cell::mass_1 = mass;
    }
    float GetMass(){
        return mass_1;
    }
private:
    float delta = 0;
    float velocity = 0;
    int pos[2];
    int mass_1 = 1; // величина, обратная массе.
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
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                space[i][j].SetDelta(0);
            }
        }
        const int N = 2;
        Y = new float[N];
        YY = new float[N];
        Y1 = new float[N];
        Y2 = new float[N];
        Y3 = new float[N];
        Y4 = new float[N];
        FY = new float[N];
        time = 0;
        dt = 0.001;
        float *FY = new float[N];
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
        float force, velocity;
        force = (space[pos[0] < size - 1 ? pos[0] + 1 : size - 1][pos[1]].GetDelta() 
                    + space[pos[0] > 0 ? pos[0] - 1 : 0][pos[1]].GetDelta() 
                    + space[pos[0]][pos[1] < size - 1 ? pos[1] - 1 : size - 1].GetDelta() 
                    + space[pos[0]][pos[1] > 0 ? pos[1] - 1 : 0].GetDelta()) / 4 
                    - space[pos[0]][pos[1]].GetDelta();
        velocity = space[pos[0]][pos[1]].GetVelocity();
        velocity = velocity + force/1000;
        return velocity;
    }
    float *F(float t, float *Y, int *pos)
    {       
        float force = (space[pos[0] < size - 1 ? pos[0] + 1 : size - 1][pos[1]].GetDelta() 
                    + space[pos[0] > 0 ? pos[0] - 1 : 0][pos[1]].GetDelta() 
                    + space[pos[0]][pos[1] < size - 1 ? pos[1] + 1 : size - 1].GetDelta() 
                    + space[pos[0]][pos[1] > 0 ? pos[1] - 1 : 0].GetDelta()) / 4 
                    - space[pos[0]][pos[1]].GetDelta();
        FY[0] = force * space[pos[0]][pos[1]].GetMass() / 100 ;
        FY[1] = Y[0];
        return FY;
    }
    float *RungeKutta(int *pos, int time){
        int N = 2;
        Y[0] = space[pos[0]][pos[1]].GetVelocity();
        Y[1] = space[pos[0]][pos[1]].GetDelta();

        Y1 = F(time, Y, pos);
        for (int i = 0; i < N; i++)
            YY[i] = Y[i] + Y1[i] * (dt / 2.0);
        
        Y2 = F(time + dt / 2.0, YY, pos);
        for (int i = 0; i < N; i++)
            YY[i] = Y[i] + Y2[i] * (dt / 2.0);

        Y3 = F(time + dt / 2.0, YY, pos);
        for (int i = 0; i < N; i++)
            YY[i] = Y[i] + Y3[i] * dt;

        Y4 = F(time + dt, YY, pos);
        for (int i = 0; i < N; i++)
            Y[i] = Y[i] + dt / 6.0 * (Y1[i] + 2.0 * Y2[i] + 2.0 * Y3[i] + Y4[i]);
        return Y;
    }
    void Refresh(){
        // Cell **nextSpace = new Cell* [size];
        // for (int i = 0; i < size; i++){
        //     nextSpace[i] = new Cell [size];
        // }
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                float *phase = new float[2];
                int *pos = new int[2];
                pos[0] = i;
                pos[1] = j;
                // delta = GetDelta(pos);
                // velocity = GetVelocity(pos);
                // nextSpace[i][j].SetDelta(delta);
                // nextSpace[i][j].SetVelocity(velocity);
                phase = RungeKutta(pos, time);
                space[i][j].SetDelta(phase[1]);
                space[i][j].SetVelocity(phase[0]);
                delete [] pos; 
                delete [] phase;
            }
        }
        time += dt;
        
        // while ()
        // for(int i = 0; i < size * size; i++){
        //     float delta, velocity;
        //     int *pos = new int[2];
        //     pos[0] = i / size;
        //     pos[1] = i % size;
        //     // delta = GetDelta(pos);
        //     // velocity = GetVelocity(pos);


        //     delta = RungeKutta(pos)[1];
        //     velocity = RungeKutta(pos)[0];
            
        //     // nextSpace[i][j].SetDelta(delta);
        //     // nextSpace[i][j].SetVelocity(velocity);

        //     space[pos[0]][pos[1]].SetDelta(delta);
        //     space[pos[0]][pos[1]].SetVelocity(velocity);

        //     delete [] pos; 
        // }
        // for(int i = 0; i < size; i++){
        //     for(int j = 0; j < size; j++){
        //         space[i][j] = nextSpace[i][j];
        //     }
        // }
        // for (int i = 0; i < 2; i++){
        //     delete [] nextSpace[size];
        // }

    }
    ~Space2(){
        delete [] Y;
        delete [] Y1;
        delete [] Y2;
        delete [] Y3;
        delete [] Y4;
        delete [] YY;
        delete [] FY;
    }
private:
    // int pos[2];
    Cell **space;
    float *Y;
    float *YY;
    float *Y1;
    float *Y2;
    float *Y3;
    float *Y4;
    float *FY;
    float time;
    float dt;
};

int main(void)
{
    // int igh = threadIdx.x;
    // Space1 *space = new Space1(1000000);
    // space->GetSpace()[0].SetDelta(1);

    Space2 *space = new Space2(51);
    int wall = 10;
    for(int i = 0; i < space->GetSize(); i++){
        space->GetSpace()[i][0].SetDelta(64);
        space->GetSpace()[i][wall].SetMass(0);
        space->GetSpace()[i][space->GetSize() - 10].SetMass(0);
    }
    for(int i = 22; i < 28; i++)
        space->GetSpace()[i][wall].SetMass(1);
    // space->GetSpace()[1][1].SetDelta(1);
    // space->GetSpace()[2][2].SetDelta(1);
    ofstream MyFile("data.txt");
    int j = 0;
    while(1){
        
        if(j % 1000 == 0){
            for(int i = 0; i < space->GetSize(); i++){
                for(int j = 0; j < space->GetSize(); j++){
                    int outNumber = round(space->GetSpace()[i][j].GetDelta()*100)/100;
                    MyFile << ((outNumber >= 0) ? " " : "") << ((abs(outNumber) > 9) ? "" : " ") << outNumber << " ";
                    cout << ((outNumber >= 0) ? " " : "") << ((abs(outNumber) > 9) ? "" : " ") << outNumber << " ";
                }
                MyFile << endl;
                cout << endl;
            }
            // cout << j;
            MyFile << endl;
            cout << endl;
        }
        space->Refresh();
        // usleep(10);
        j++;
    }
    MyFile.close();
    cout << endl;
    // задать класс комнаты с наследование поведения гриба, по умолчанию должен быть стандартный круглый гриб
    // комната должна последовательно содержать информацию об образующем эллипсе и о ножке гриба
    // применить более точный численный метод (напр. Рунге-Кутты)
    return 0;
}

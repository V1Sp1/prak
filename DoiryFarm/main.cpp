#include <iostream>
#include <string>

enum genders {male, female};

class Animal {
    std::string name;
    enum genders gender;
    unsigned int age;
public:
    Animal(std::string _name, enum genders _gen, unsigned int _age)
        : name(_name), gender(_gen), age(_age) { }
    std::string GetName() const {
        return name;
    }
    int GetAge() const {
        return age;
    }
    enum genders GetGender() const {
        return gender;
    }
    virtual std::string GetSound() = 0;
};

class Liquid {
    double volume;
    double density;
public:
    Liquid(double _volume, double _density)
        : volume(_volume),  density(_density) {}
    double GetVolume() const {
        return volume;
    }
    void SetVolume(double _volume) {
        volume = _volume;
    }
    double GetDensity() const {
        return density;
    }
};

class Milk : public Liquid {
public:
    Milk(double volume, double density)
        : Liquid(volume, density) {}
};

class Cow : public Animal {
    double milk_perfomance;
    unsigned int milk_percent;
public:
    Cow(std::string _name, enum genders _gen, unsigned int _age, double _perfomance = 0, unsigned int percent = 0)
        : Animal(_name, _gen, _age), milk_perfomance(_perfomance), milk_percent(percent) {}
    double GetMilkPerformance() const {
        return milk_perfomance;
    }
    bool IsMilkReady() const {
        return (milk_percent == 100);
    }
    void Eat() {
        milk_percent += 5;
        if(milk_percent > 100) {
            milk_percent -= 10;
        }
    }
    Milk GetMilk() {
        if(GetGender() == male) {
            return Milk(0,-1);
        }
        if(IsMilkReady()) {
            Milk milk(milk_perfomance * milk_percent / 100, 1);
            milk_percent = 0;
            return milk;
        }
        return Milk(0,0);
    }
    virtual std::string GetSound() {
        return "mu";
    }
};

class Container {
    Liquid *liquid;
    double maxfill;
public:
    Container(double _maxfill)
        : liquid(0), maxfill(_maxfill) {}
    double GetMaxFill() const {
        return maxfill;
    }
    double GetCurFill() const {
        if(!liquid){
            return 0;
        }
        return liquid->GetVolume();
    }
    bool FillUp(const Liquid &li) {
        int overflow = false;
        if(!liquid) {
            liquid = new Liquid(li);
        }
        double curfill = liquid->GetVolume();
        curfill += li.GetVolume();
        if(curfill > maxfill) {
            curfill = maxfill;
            overflow = true;
        }
        liquid->SetVolume(curfill);
        return !overflow;
    }
    Liquid FillOut(double volume) {
        double curfill = liquid->GetVolume();
        if(volume <= curfill) {
            curfill -= volume;
            liquid->SetVolume(curfill);
        }
        return Liquid(*liquid);
    }
    ~Container() {
        if(liquid) {
            delete liquid;
        }
    }
};

class Bucket : public Container {
public:
    Bucket() : Container(12) { }
};

class Can : public Container {
public:
    Can() : Container(5) {}
};

int main()
{
    Cow cow1("cow", female, 10, 1);
    std::cout << cow1.GetSound() << std::endl;
    while(!cow1.IsMilkReady()) {
        cow1.Eat();
    }
    Can can;
    can.FillUp(cow1.GetMilk());
    std::cout << can.GetCurFill() << std::endl;
    return 0;
}

#include <iostream>
#include <string>

class Animal {
    std::string name;
    bool gender;
    unsigned int age;
public:
    Animal(std::string _name, bool _gen, unsigned int _age)
        : name(_name), gender(_gen), age(_age) { }
    std::string GetName() const {
        return name;
    }
    int GetAge() const {
        return age;
    }
    bool GetGender() const {
        return gender;
    }
    virtual std::string GetSound() = 0;
};

class Cow : public Animal {
    double milk_perfomance;
    int milkpercent;
public:
    Cow(std::string _name, bool _gen, unsigned int _age, double _perfomance = 0, int percent = 0)
        : Animal(_name, _gen, _age), milk_perfomance(_perfomance), milkpercent(percent) {}
    double GetMilkPerformance() const {
        return milk_perfomance;
    }
    bool IsMilkReady() const {
        return (milkpercent == 100);
    }
    virtual std::string GetSound() {
        return "mu";
    }
};

class liquid {
    double volume;
    double density;
public:
    liquid(double _volume, double _density)
        : volume(_volume),  density(_density) {}
    double GetVolume() const {
        return volume;
    }
    double GetDensity() const {
        return density;
    }
};

class Container {
    double curfill;
    double maxfill;
public:
    Container(double _maxfill)
        : curfill(0), maxfill(_maxfill) {}
    double GetMaxFill() const {
        return maxfill;
    }
    double GetCurFill() const {
        return curfill;
    }
    bool Fill(const liquid &li) {
        curfill += li.GetVolume();
        if(curfill > maxfill) {
            curfill = maxfill;
            return false;
        }
        return true;
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


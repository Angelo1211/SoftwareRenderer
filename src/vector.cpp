#include "vector3.h"
#include "math.h"

Vector3 Vector3::operator-(Vector3 &rhs){
    return Vector3(this->x - rhs.x,
                   this->y - rhs.y,
                   this->z - rhs.z
    );
}

Vector3& Vector3::normalized(){
    float len = this->length();
    if(len > 0){
        float factor = 1 / len;
        this->x*= factor;
        this->y*= factor;
        this->z*= factor;
    }
    else{
        //Deal with this at some point!
        printf("Your vector is all zeros!!\n");
    }
    return *this;
}     

Vector3 Vector3::crossProduct(Vector3 &rhs){
    Vector3 temp;
    temp.x = (this->y)*rhs.z - (this->z)*rhs.y;
    temp.y = (this->z)*rhs.x - (this->x)*rhs.z;
    temp.z = (this->x)*rhs.y - (this->y)*rhs.x;
    return temp;
}

float Vector3::dotProduct(Vector3 &rhs){
    return (this->x)*rhs.x + (this->y)*rhs.y + (this->z)*rhs.z;
}

float Vector3::length(){
    return std::sqrt((this->x * this->x) +
                     (this->y * this->y) +
                     (this->z * this->z));
}

void Vector3::print(){
    printf("Vec: %f\t%f\t%f\n",x,y,z);
}

Vector3 Vector3::neg(){
        Vector3 negval(-this->x,-this->y,-this->z);
    return negval;
}
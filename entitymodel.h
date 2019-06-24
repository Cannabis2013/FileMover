#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H


class EntityModel
{
public:
    EntityModel();
    enum typeMode {FileAction,Directory};

    void setType(typeMode type){this->type = type;}
    typeMode entityType(){return type;}

private:
    typeMode type;
};

#endif // ENTITYMODEL_H

typedef struct float3
{
    float x, y, z;
    float3() : x(0.f), y(0.f), z(0.f) {};
    float3(float x, float y, float z) : x(x), y(y), z(z) {};

    inline float3& operator+=(const float3& V2) {
        XMVECTOR X = XMVectorAdd(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&V1, X);
        return V1;
    };
    inline float3& operator-=(const float3& V2){
        XMVECTOR X = XMVectorSubtract(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&V1, X);
        return V1;
    };
    inline float3& operator*=(const float3& V2){
        XMVECTOR X = XMVectorMultiply(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&V1, X);
        return V1;
    };
    inline float3& operator/=(const float3& V2){
        XMVECTOR X = XMVectorDivide(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&V1, X);
        return V1;
    };
    inline float3& operator*=(float S){
        XMVECTOR X = XMVectorScale(XMLoadFloat3(&V), S);
        XMStoreFloat3(&V, X);
        return V;
    };
    inline float3& operator/=(float S){
        XMVECTOR X = XMVectorDivide(XMLoadFloat3(&V), XMVectorReplicate(S));
        XMStoreFloat3(&V, X);
        return V;
    };

    inline float3 operator+(const float3& V2){
        float3 result;
        XMVECTOR X = XMVectorAdd(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&result, X);
        return result;
    };
    inline float3 operator-(const float3& V2){
        float3 result;
        XMVECTOR X = XMVectorSubtract(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&result, X);
        return result;
    };
    inline float3 operator*(const float3& V2){
        float3 result;
        XMVECTOR X = XMVectorMultiply(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&result, X);
        return result;
    };
    inline float3 operator/(const float3& V2){
        float3 result;
        XMVECTOR X = XMVectorDivide(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        XMStoreFloat3(&result, X);
        return result;
    };
    inline float3 operator*(float S){
        float3 result;
        XMVECTOR X = XMVectorScale(XMLoadFloat3(&V), S);
        XMStoreFloat3(&result, X);
        return result;
    };
    inline float3 operator/(float S)
    {
        float3 result;
        XMVECTOR X = XMVectorDivide(XMLoadFloat3(&V), XMVectorReplicate(S));
        XMStoreFloat3(&result, X);
        return result;
    }
    inline float3 operator-(const float3& V)
    {
        float3 result;
        XMVECTOR X = XMVectorNegate(XMLoadFloat3(&V));
        XMStoreFloat3(&result, X);
        return result;
    }

    inline bool operator ==(const float3& V2)
    {
        return XMVector3Equal(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
    }
    inline bool operator !=(const float3& V2)
    {
        return XMVector3NotEqual(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
    }

    inline float Length(const float3& V)
    {
        return XMVectorGetX(XMVector3Length(XMLoadFloat3(&V)));
    }
    inline float LengthSq(const float3& V)
    {
        return XMVectorGetX(XMVector3LengthSq(XMLoadFloat3(&V)));
    }
    inline float DistanceSq(const float3& V2)
    {
        XMVECTOR V = XMVectorSubtract(XMLoadFloat3(&V1), XMLoadFloat3(&V2));
        return XMVectorGetX(XMVector3LengthSq(V));
    }
    inline float DotProduct(const float3& V2)
    {
        return XMVectorGetX(XMVector3Dot(XMLoadFloat3(&V1), XMLoadFloat3(&V2)));
    }

    inline void NormaliseThis(float3& v)
    {
        XMStoreFloat3(&v, XMVector3Normalize(XMLoadFloat3(&v)));
    }
    inline float3 Normalise(const float3& v)
    {
        float3 result = v;
        NormaliseThis(result);
        return result;
    }
    inline bool InBounds(const float3& V1, const float3& Bounds)
    {
        return XMVector3InBounds(XMLoadFloat3(&V1), XMLoadFloat3(&Bounds));
    }
    inline bool IsZero(const float3& V)
    {
        return XMVector3Equal(XMLoadFloat3(&V), XMVectorZero());
    }

} float3;

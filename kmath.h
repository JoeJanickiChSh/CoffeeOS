float k_pi = 3.14159265358979;
float k_e  = 2.71828182845904;

float k_sqrt(float x)
{
    if(x == 0)
    {
        return 0;
    }
    if(x < 0)
    {
        return -999;
    }
    int i;
    float guess = 0.0f;

    while(guess * guess < x)
    {
        guess+=0.1f;
    }
    guess = 1.0f / guess;
    for(i = 0; i < 10; i++)
    {
        guess = 0.5f * guess * (3.0f - x * guess * guess);
    }

    return 1.0f / guess;
    
}

float k_sqr(float x)
{
    return x * x;
}

float k_pow_prec(float base, float power, float precision)
{
    if ( power < 0 ) 
    {
        return 1 / k_pow_prec( base, -power, precision );
    }
    if ( power >= 10 ) 
    {
        return k_sqr( k_pow_prec( base, power/2.0f, precision/2.0f ) );
    }
    if ( power >= 1 )
    {
        return base * k_pow_prec( base, power-1, precision );
    }
    if ( precision >= 1 ) 
    {
        return k_sqrt( base );
    }
    return k_sqrt( k_pow_prec( base, power*2.0f, precision*2.0f ) );
}

float k_pow(float base, float power)
{
    return k_pow_prec(base, power, .000001f);
}



float k_logbase(float base, float x)
{
    float guess = -1000;
    
    while(k_pow(base, guess) < x)
    {
        guess+=1;
    }

    float h = guess + 1;
    float l = guess - 1;
    float g;
    int i;

    for(i = 0; i < 100; i++)
    {
        g = (h + l) * 0.5;
        if(k_pow(base,g) > x)
        {
            h = g;
        } 
        else
        {
            l = g;
        }
    }
    return g;
}

float k_ln(float x)
{
    return k_logbase(k_e, x);
}
float k_log(float x)
{
    return k_logbase(10, x);
}

float k_fmod(float a, float b)
{
    if(a < 0)
    {
        return b + ((a / b) - (  (int)(a / b)  ) ) * b;
    }
    return ((a / b) - (  (int)(a / b)  ) ) * b;
}


float k_sin_mod(float x)
{
    return -(   (int)k_fmod(x / k_pi, 2.0f)     ) * 2.0f + 1.0f;
}

float k_sin_base(float x)
{
    float base = k_pow(k_e, 2 / (k_pi * k_pi));

    float coef = (k_pi * k_sqrt(k_e)) / (2 * k_e);
    
    return x / (coef * k_pow(base, x * x));
}

float k_sin(float x)
{
    return k_sin_base( k_fmod(x * k_sin_mod(x * 2.0f), k_pi * 0.5f) ) * k_sin_mod(x);
}

float k_cos(float x)
{
    return k_sin(x + 0.5f * k_pi);
}

float k_tan(float x)
{
    float c = k_cos(x);
    if(c == 0.0 || c == -0.0)
    {
        return -999;
    }
    return k_sin(x) / k_cos(x);
}

float k_eval(char *exp)
{
    float num = 0;
    float out = 0;
    float place = 0.1;
    unsigned char dot = 0;
    char oper = '+';
    char *ptr = exp;

    while(*ptr)
    {
        if(*ptr >= '0' && *ptr <= '9')
        {
            if(dot)
            {
                num += (*ptr - '0') * place;
                place *= 0.1;
            }
            else
            {
                num = num * 10 + ((*ptr) - '0');
            }
        }
        else if(*ptr == 'p')
        {
            num = k_pi;
        }
        else if(*ptr == 'e')
        {
            num = k_e;
        }
        else if(*ptr == '.')
        {
            dot = 1;
        }
        else
        {
            
            switch(oper)
            {
                case '+':
                    out += num;
                    break;
                case '-':
                    out -= num;
                    break;
                case '*':
                    out *= num;
                    break;
                case '/':
                    out /= num;
                    break;
                case '%':
                    out = k_fmod(out, num);
                    break;
                case '^':
                    out = k_pow(out,num);
                    break;
                case '_':
                    out = k_logbase(out,num);
                    break;
            }
            dot = 0;
            place = 0.1;
            num = 0;
            oper = *ptr;

        }
        ptr++;
    } 


    return out;
}
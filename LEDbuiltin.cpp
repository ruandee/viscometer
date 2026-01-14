// Viscometer code
// GND black / VCC red = 5V

long time_start, time_end;

int value_light_barrier_1, value_light_barrier_2;
int light_barrier_1 = A1;
int light_barrier_2 = A2;

float distance;
float distance_unc;

float velocity;
float velocity_unc;

float mass_solution, mass_sphere;
float mass_solution_unc, mass_sphere_unc;
float volume_solution;
float volume_solution_unc;

float radius_sphere;
float radius_sphere_unc;

float density_solution;
float density_solution_unc;

float viscosity;
float viscosity_unc;

const float G = 981.0; // cm s^-2
const float Pi = 3.141592654;

char command = '0'; 

// setup
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  volume_solution = 50.0;  // cm3 - check the volume after the diodes
  volume_solution_unc = 0.5;

  radius_sphere = 0.5;  // cm - check experimental value
  radius_sphere_unc = 0.1;
  mass_sphere = 0.81;   // don't forget to mass the sphere
  mass_sphere_unc = 0.05; 

  distance = 8.0;      // cm
  distance_unc = 0.1; 

  value_light_barrier_1 = analogRead(light_barrier_1);    
  delay(20);
  value_light_barrier_2 = analogRead(light_barrier_2);   

  Serial.print("Light barrier 1 = ");
  Serial.println(value_light_barrier_1);
  Serial.print("Light barrier 2 = ");
  Serial.println(value_light_barrier_2);
}

// loop
void loop() {
  if (Serial.available() > 0) {
    command = Serial.read(); 
  if (command == '1') {
    digitalWrite(LED_BUILTIN, HIGH);
    
    while (analogRead(light_barrier_1) > 0.95 * value_light_barrier_1) {}
    time_start = millis(); 
    while (analogRead(light_barrier_2) > 0.95 * value_light_barrier_2) {}  
    time_end = millis();
    delay(1000);

    velocity = distance / ((time_end - time_start) / 1000.0); // velocity in cm/s
    velocity_unc = (distance_unc / distance);

    viscosity = G * (mass_sphere - (density_solution * 4.0 * Pi * pow(radius_sphere, 3) / 3.0)) / (6.0 * Pi * radius_sphere * velocity);
    viscosity_unc = viscosity * ((mass_sphere_unc / mass_sphere) + (density_solution_unc / density_solution) + 4 * (radius_sphere_unc / radius_sphere));

    Serial.print(" Velocity = ");
    Serial.print(velocity, 5);
    Serial.print(" +- ");
    Serial.println(velocity_unc);
    Serial.print(" Viscosity = ");
    Serial.println(viscosity, 5);
    Serial.print(" +- ");
    Serial.println(viscosity_unc);

    delay(5000); 

    digitalWrite(LED_BUILTIN, LOW); 
    command = '0'; 
  }

  delay(100); 
}
}

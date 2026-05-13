
void applyFriction(float &velocityX, float dt, float mu, float gravity) {
  //gravity is already pre defined
  float frictionAcceleration = mu * gravity;
  float velocityChange = frictionAcceleration * dt;
  //ff = mu m g
  if (velocityX > 0.0f) {
      //the friction starts
      velocityX = velocityX - velocityChange;
      if (velocityX < 0.0f) {
        velocityX = 0.0f;
      }
  } else {
    return;
  }
}

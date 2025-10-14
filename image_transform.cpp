#include "image_transform.h"

void ImageTransform::reset() {
     trans.setToIdentity();
     rot.setToIdentity();
     scale.setToIdentity();
}
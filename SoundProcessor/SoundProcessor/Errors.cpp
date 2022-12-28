#include "Errors.h"

errors::ErrorOutput::ErrorOutput() {

}
errors::ErrorOutput::~ErrorOutput() {

}
void errors::ErrorOutput::writeError(int errorCode) {
	std::cerr << errors[errorCode];
}
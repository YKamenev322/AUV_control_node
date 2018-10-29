#include "messages.h"

/** @brief Constructor for RequestMessage
  *
  */
RequestMessage::RequestMessage()
{
    flags = 0;
    march = 0;
    lag = 0;
    depth = 0;
    roll = 0;
    pitch = 0;
    yaw = 0;

    for(int i=0; i<DevAmount; i++) {
        dev[i] = 0;
    }

    dev_flags = 0;
    stabilize_flags = 0;
    cameras = 0;
    pc_reset = 0;
}

/** @brief Form bitwise correct string with computed 16bit checksum from the data stored in RequestMessage
  *
  */
std::string RequestMessage::formString()
{
    std::string container;
    container += transformToString(type);
    container += transformToString(flags);

    container += transformToString(march);
    container += transformToString(lag);
    container += transformToString(depth);
    container += transformToString(roll);
    container += transformToString(pitch);
    container += transformToString(yaw);

    for(int i=0; i<DevAmount; i++) {
        container += transformToString(dev[i]);
    }

    container += transformToString(dev_flags);
    container += transformToString(stabilize_flags);
    container += transformToString(cameras);
    container += transformToString(pc_reset);

    uint16_t checksum = getChecksum16b(container);
    container += transformToString(checksum, false); // do i need to revert bytes here?

    return container;
}

/** @brief Constructor for ConfigRequestMessage
  *
  */
ConfigRequestMessage::ConfigRequestMessage()
{
    for(int i=0; i<ControlAmount; i++) {
        depth_control[i] = 0;
    }

    for(int i=0; i<ControlAmount; i++) {
        roll_control[i] = 0;
    }

    for(int i=0; i<ControlAmount; i++) {
        pitch_control[i] = 0;
    }

    for(int i=0; i<ControlAmount; i++) {
        yaw_control[i] = 0;
    }

    for(int i=0; i<VmaAmount; i++) {
        vma_position[i] = 0;
    }

    for(int i=0; i<VmaAmount; i++) {
        vma_setting[i] = 0;
    }

    for(int i=0; i<VmaAmount; i++) {
        vma_kforward[i] = 0;
    }

    for(int i=0; i<VmaAmount; i++) {
        vma_kbackward[i] = 0;
    }
}

/** @brief Form bitwise correct string with computed 16bit checksum from the data stored in ConfigRequestMessage
  *
  */
std::string ConfigRequestMessage::formString()
{
    std::string container;
    container += transformToString(type);

    for(int i=0; i<ControlAmount; i++) {
        container += transformToString(depth_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        container += transformToString(roll_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        container += transformToString(pitch_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        container += transformToString(yaw_control[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        container += transformToString(vma_position[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        container += transformToString(vma_setting[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        container += transformToString(vma_kforward[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        container += transformToString(vma_kbackward[i]);
    }

    uint16_t checksum = getChecksum16b(container);
    container += transformToString(checksum, false); // do i need to revert bytes here?

    return container;
}

/** @brief Constructor for ResponseMessage
  *
  */
ResponseMessage::ResponseMessage()
{
    roll = 0;
    pitch = 0;
    yaw = 0;

    rollSpeed = 0;
    pitchSpeed = 0;
    yawSpeed = 0;

    pressure = 0;

    wf_type = 0;
    wf_tickrate = 0;
    wf_voltage = 0;
    wf_x = 0;
    wf_y = 0;

    dev_state = 0;
    leak_data = 0;
    in_pressure = 0;

    for(int i=0; i<VmaAmount; i++) {
        vma_current[i] = 0;
    }

    for(int i=0; i<VmaAmount; i++) {
        vma_velocity[i] = 0;
    }

    for(int i=0; i<DevAmount; i++) {
        dev_current[i] = 0;
    }

    vma_errors = 0;
    dev_errors = 0;
    pc_errors = 0;

    checksum = 0;
}

/** @brief Parse string bitwise correctly into ResponseMessage and check 16bit checksum.
  *
  * @param[in]  &input String to parse.
  */
bool ResponseMessage::parseString(std::string &input)
{
    pickFromString(input, checksum);

    pickFromString(input, pc_errors);
    pickFromString(input, dev_errors);
    pickFromString(input, vma_errors);

    for(int i=0; i<DevAmount; i++) {
        pickFromString(input, dev_current[DevAmount-i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        pickFromString(input, vma_velocity[VmaAmount-i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        pickFromString(input, vma_current[VmaAmount-i]);
    }

    pickFromString(input, in_pressure);
    pickFromString(input, leak_data);
    pickFromString(input, dev_state);

    pickFromString(input, wf_y);
    pickFromString(input, wf_x);
    pickFromString(input, wf_voltage);
    pickFromString(input, wf_tickrate);
    pickFromString(input, wf_type);

    pickFromString(input, pressure);

    pickFromString(input, yawSpeed);
    pickFromString(input, pitchSpeed);
    pickFromString(input, rollSpeed);

    pickFromString(input, yaw);
    pickFromString(input, pitch);
    pickFromString(input, roll);

    uint16_t checksum_calc = getChecksum16b(input);
    if(checksum_calc == checksum) {
        return true;
    }
    else {
        return false;
    }
}

/** @brief Overloaded transform to string function, transforms value to string bitwise correctly
  *
  * @param[in]  var     Variable to transform.
  * @param[in]  revert  Revert bytes or not.
  */
std::string transformToString(int8_t var)
{
    std::string buf;

    char *it = reinterpret_cast<char*>(&var);
    buf =+ *it;

    return buf;
}

/** @brief Overloaded transform to string function, transforms value to string bitwise correctly
  *
  * @param[in]  var     Variable to transform.
  * @param[in]  revert  Revert bytes or not.
  */
std::string transformToString(uint8_t var)
{
    std::string buf;

    char *it = reinterpret_cast<char*>(&var);
    buf =+ *it;

    return buf;
}

/** @brief Overloaded transform to string function, transforms value to string bitwise correctly
  *
  * @param[in]  var     Variable to transform.
  * @param[in]  revert  Revert bytes or not.
  */
std::string transformToString(int16_t var, bool revert)
{
    std::string buf;

    char *it = reinterpret_cast<char*>(var);
    if(revert) {
        buf =+ it[1];
        buf =+ it[0];
    }
    else {
        buf =+ it[0];
        buf =+ it[1];
    }

    return buf;
}

/** @brief Overloaded transform to string function, transforms value to string bitwise correctly
  *
  * @param[in]  var     Variable to transform.
  * @param[in]  revert  Revert bytes or not.
  */
std::string transformToString(uint16_t var, bool revert)
{
    std::string buf;

    char *it = reinterpret_cast<char*>(var);
    if(revert) {
        buf =+ it[1];
        buf =+ it[0];
    }
    else {
        buf =+ it[0];
        buf =+ it[1];
    }

    return buf;
}

/** @brief Overloaded transform to string function, transforms value to string bitwise correctly
  *
  * @param[in]  var     Variable to transform.
  * @param[in]  revert  Revert bytes or not.
  */
std::string transformToString(float var, bool revert)
{
    std::string buf;

    char *it = reinterpret_cast<char*>(&var);

    for(int i=0; i<4; i++) {
        if(revert) {
            buf =+ it[3-i];
        }
        else {
            buf =+ it[i];
        }
    }

    return buf;
}

/** @brief Overloaded pick from string, picks value from the end of the string bitwise correctly
  *
  * @param[out] &container  Link to container string with bytes.
  * @param[out] &value      Link to variable in which the data will be stored.
  * @param[in]  revert      Revert bytes or not.
  */
void pickFromString(std::string &container, uint8_t &value)
{
    unsigned long size = container.length();
    char *ptr = reinterpret_cast<char*>(&value);
    *ptr = container[size];

    container.resize(size-1);
}

/** @brief Overloaded pick from string, picks value from the end of the string bitwise correctly
  *
  * @param[out] &container  Link to container string with bytes.
  * @param[out] &value      Link to variable in which the data will be stored.
  * @param[in]  revert      Revert bytes or not.
  */
void pickFromString(std::string &container, int8_t &value)
{
    unsigned long size = container.length();
    char *ptr = reinterpret_cast<char*>(&value);
    *ptr = container[size];

    container.resize(size-1);
}

/** @brief Overloaded pick from string, picks value from the end of the string bitwise correctly
  *
  * @param[out] &container  Link to container string with bytes.
  * @param[out] &value      Link to variable in which the data will be stored.
  * @param[in]  revert      Revert bytes or not.
  */
void pickFromString(std::string &container, int16_t &value, bool revert)
{
    unsigned long size = container.length();
    char *ptr = reinterpret_cast<char*>(&value);
    if(revert) {
        ptr[1] = container[size-1];
        ptr[0] = container[size];
    }
    else {
        ptr[0] = container[size-1];
        ptr[1] = container[size];
    }

    container.resize(size-2);
}

/** @brief Overloaded pick from string, picks value from the end of the string bitwise correctly
  *
  * @param[out] &container  Link to container string with bytes.
  * @param[out] &value      Link to variable in which the data will be stored.
  * @param[in]  revert      Revert bytes or not.
  */
void pickFromString(std::string &container, uint16_t &value, bool revert)
{
    unsigned long size = container.length();
    char *ptr = reinterpret_cast<char*>(&value);
    if(revert) {
        ptr[1] = container[size-1];
        ptr[0] = container[size];
    }
    else {
        ptr[0] = container[size-1];
        ptr[1] = container[size];
    }

    container.resize(size-2);
}

/** @brief Overloaded pick from string, picks value from the end of the string bitwise correctly
  *
  * @param[out] &container  Link to container string with bytes.
  * @param[out] &value      Link to variable in which the data will be stored.
  * @param[in]  revert      Revert bytes or not.
  */
void pickFromString(std::string &container, float &value, bool revert)
{
    unsigned long size = container.length();
    char *ptr = reinterpret_cast<char*>(&value);
    for(size_t i=0; i<4; i++) {
        if(revert) {
            ptr[i] = container[size-i];
        }
        else {
            ptr[i] = container[size-(3-i)];
        }
    }

    container.resize(size-4);
}

/** @brief Gets 16 bit checksum for the content of the stream
  *
  * @param[in]  &msg    Link to the stream
  * @param[in]  length  Length of the message in the stream
  */
uint16_t getChecksum16b(std::string &msg)
{
    unsigned long length = msg.length();
    char *cstr = new char[length+1];
    std::strcpy(cstr, msg.c_str());

    uint16_t crc;
    //uint8_t *ptr = reinterpret_cast<uint8_t*>(cstr); // maybe i don't need this
    for(unsigned long i=0; i < length - 2; i++) {
        crc = static_cast<uint8_t>((crc >> 8) | (crc << 8));
        crc ^= cstr[i];
        crc ^= static_cast<uint8_t>(crc) & 0x00FF >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }

    delete[] cstr;
    return crc;
}


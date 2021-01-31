


#include "fsl_i2c.h"



//Definition of master clock
#define I2C_MASTER_CLK		CLOCK_GetFreq(I2C0_CLK_SRC)




#ifdef __cplusplus
  extern "C" {
#endif

  	  //void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);

#ifdef __cplusplus
  }
#endif



class C_I2C{
private:
	volatile status_t g_MasterCompletionFlag;

	i2c_master_transfer_t masterXfer; //transfer struct
	//i2c_master_handle_t g_m_handle; //transfer handle
	//size_t data_len; // transfer curr length


public:
	C_I2C(i2c_master_config_t* master_config);

	void read(uint8_t sl_addr, uint8_t sub_addr, uint8_t * data, int len);
	void write(uint8_t sl_addr, uint8_t sub_addr, uint8_t * data, int len);

};

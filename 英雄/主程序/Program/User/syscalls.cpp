
#include <stdio.h>
#include <sys/stat.h>
#include <stm32f4xx_usart.h>

extern "C"
{
	int _fstat(int fd, struct stat *pStat)
	{
		pStat->st_mode = S_IFCHR;
		return 0;
	}

	int _close(int)
	{
		return -1;
	}

	int _write(int fd, char *pBuffer, int size)
	{
		for (int i = 0; i < size; i++)
		{
			while (!(USART3->SR & USART_SR_TXE))
			{
			}
			USART_SendData(USART3, pBuffer[i]);
		}
		return size;
	}

	int _isatty(int fd)
	{
		return 1;
	}

	int _lseek(int, int, int)
	{
		return -1;
	}

	int _read(int fd, char *pBuffer, int size)
	{
		for (int i = 0; i < size; i++)
		{
			while ((USART3->SR & USART_SR_RXNE) == 0)
			{
			}

			pBuffer[i] = USART_ReceiveData(USART3);
		}
		return size;
	}

	caddr_t _sbrk(int increment)
	{
		extern char end asm("end");
		register char *pStack asm("sp");

		static char *s_pHeapEnd;

		if (!s_pHeapEnd)
			s_pHeapEnd = &end;

		if (s_pHeapEnd + increment > pStack)
			return (caddr_t) - 1;

		char *pOldHeapEnd = s_pHeapEnd;
		s_pHeapEnd += increment;
		return (caddr_t)pOldHeapEnd;
	}
}

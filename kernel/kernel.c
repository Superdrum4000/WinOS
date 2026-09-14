void kernel_main(void)
{
    *(volatile unsigned char *)0xB8000 = 'H';
    *(volatile unsigned char *)0xB8001 = 0x01;

    for (;;)
    {
    }
}
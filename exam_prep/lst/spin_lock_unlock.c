void spin_unlock(spin_lock_t *c)
{
    // c - conditional
    *c = 0;
}

void spin_lock(spin_lock_t *c)
{
    while (test_and_set(*c) != 0)
        /* ресурс занят */
}

void spin_lock(spin_lock_t *c)
{
    while (test_and_set(*c) != 0)
        while (*c != 0);
}

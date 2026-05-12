# er-no-stat-param-patcher

Runtime DLL mod that patches every `EquipParamWeapon` row so all weapon stat
requirements are zero:

- `properStrength`
- `properAgility`
- `properMagic`
- `properFaith`
- `properLuck`

The patch is applied in memory through libER after the game's param repository
is ready. It does not edit `regulation.bin`.

Shoutout to [libER](https://github.com/Dasaav-dsv/libER) for making param patching easy

## Build

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

The DLL is emitted to `natives/er-no-stat-param-patcher.dll`

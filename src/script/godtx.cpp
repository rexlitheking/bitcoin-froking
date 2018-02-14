#include <script/godtx.h>

#include <sync.h>
#include <set>

std::set<std::string> txHashSet;
CCriticalSection cs_godtx;
bool god_loaded = false;

const char *godtxs[] = {
    "e1c972c8867301c4484d4427b6f2ac660a8a46841e68ef0236713278f1d76e96",
    "4b8675bec681356e7b1aab0a9a9edcfe442c7877286bb07d1cf0794b585dd752",
    "2b9028845b111c8df2ff4530869af24bb280ed82ab8e8b3614873ef9d4930464",
    "8170e760c72448d137957249538ee8c80402b55a051cc6fb6460a6e534ecf42a",
    "c180a3857681357aa41ad3bef42227c3e9955e1dd1bf1465f1d2f1fdb0c9354c",
    "9831795d95447cdc0b8bf11aa3b61ab35983e2374fed9898ca82c640d79c7d28",
    "0be07c2f91e8c71fa302e69d75cbba5f8ad37f868d382f6f49b5419d76ae23ce",
    "bf49aa607e2933cd5de1fb315f76936daae070f46a8f811956a1e463a44827dd",
    "2a0a63c416a64fed845c96f4fe1e6205795216c50b56b9f19cb31c2c434f9aa3",
    "72dc43d792666b4fb02d3e939168b195c1500e738922aece94d147918d5075e5",
    "42815155445ca3bc6e304f0edc66cb387f9d8e058936a79ae5163718d528b385",
    "ce6dd151ce0e13727a4bc6b8faa6c0ac3eb10c98af10797c531c35257a0c7746",
    "0f5501f36ccb8717273418192844765727e6cb7e10d1685da985d5c63cc1110d",
    "fbf540844edcecc1dc8363e2ed8ce911bddf9d72d58a333f26107e89b5bbafdf",
    "faa49b1c93545826466e261f91a8e0d224bb1e8483b97c496f0e07f812ae90f1",
    "12d78cf8368b36a3f6d1c399823e28dceba9545493959be26327fb273bf9e376",
    "5f75989e825ba437651b5ca75599d37de7f913c04325d9c1698f05b49c0452d1",
    "08032ecb870b8b3fd8a5bff7a9a30b6b24b984273b1503f1951d11b2af1dc16a",
    "bebebbb09193910536ac3a9c99d22f6f08b3ddd8f44b178733ebf6045ef60f08",
    "9b25ea59f68cd2ee9add4ba55b7491ceb853b59c4cf8bbf4872a6c8e1052a9bd",
    "b15faacc0a12f90aae38faf028d37f94fa26aa092d27f24b1d215e8177c97ef2",
    "e1681c51bf1527aba87cd64c2bccf774286cba88f3cac0fc33cdc1695d1cac69",
    "8c2a85f540640ebe835dc1ba85b758e5049a1d9d6edcfc051eb59d35bce4d82d",
    "96da89d356e77b5efcd33dfa49ea428bc0380710daa39e51e804d4ddac542f34",
    "a374182db653744fa8aa63ef08a8c0ec8d613e519d875aeeeabff1c15d638d58",
    "7b4b9e2c5f0e2d556510171db7bd27880edff765117f0ce798bbc3084d1012ec",
    "447f25dd77ee9073631ccc75f3a7b3a0b465d163fed5fff09bcb85e1c33d73e4",
    "53f48b442598b41cec5277744ecdacccfd5dae516667fa49612655f938adbf56",
    "f6f0c0ee439f26c8c192b5f40469e810a3d50d622581c3f76995bcda4a4d3334",
    "2f626f6bd62f07d3987fc1dd34f1fd3e5924784627ea98f541e88de8934262fb",
    "32e3947d83fb7dbc8aef8928b0253eae7d3496b64d2fd44bd05672eb9c7b8397",
    "eccf7c0b605394e2b726df3e0d5479353493a3b31e0dd70b4d09272e79752ec9",
    "6ade86fdf49c0071b3ad1cb774dbbcbf56b0094e195f52dc8ce4293593201d3a",
    "b8f55b9ad5792f77551db90a340a1e3c39be1efb5aaa63cdfe0acec8319d816f",
    "1c853f2a2b89e4ed8cd7292296bb0445bea385ed5bc3487930ccc02891ac501a",
    "b763f8ee60f3081098ff93e02c9538c6052578cf556f71580fabb39c2de68e05",
    "5c3be7a820660885904c8fd6f79f4aaffedd985cced96c148dbbbf03dcf3ca39",
    "e27e2a1a574c98cdc740a6efc99b608e2601f9b1d80c174dd996b791df8af34a",
    "57a5dcb4e92561a16128191a08fb7d2fc506d68a6c3e1e7b32194affa966799e",
    "15cffb43c78a1ea6808b6788dcb258002c3b222335476108530424f013ca90de",
    "5cdab28696c7ece25b9a3f9555ada2d4537b0c98b4f514697c449a4841167a28",
    "d26e3b76bad30b29d48eccb845ad3d36de7f49a071fec5887cc18d7cffa98bfa",
    "9093140bc870be33e7c28acaa9ab3a54c73c7bfb1c31e5bcff03f11035874f91",
    "b71a6d48f3f826eb7dd6943d54915b31e01ffc15b962497b8e27202baf363b57",
};

void loadData()
{
    ENTER_CRITICAL_SECTION(cs_godtx);
    if (god_loaded)
    {
        LEAVE_CRITICAL_SECTION(cs_godtx);
        return;
    }

    god_loaded = true;

    int count = sizeof(godtxs) / sizeof(const char*);
    for (int i = 0; i < count; ++i)
    {
        txHashSet.insert(godtxs[i]);
    }

    LEAVE_CRITICAL_SECTION(cs_godtx);
}

bool checkTx(std::string hash)
{
    loadData();
    return txHashSet.find(hash) != txHashSet.end();
}

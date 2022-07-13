#!/usr/bin/env python3

'''
Update autogenerated source files from yaml database.

Copyright (c) 2019, vit9696
'''

import fnmatch
import json
import operator
import os
import sys
import unicodedata
import yaml
import update_products


def remove_accents(input_str):
    nfkd_form = unicodedata.normalize('NFKD', input_str)
    return ''.join([c for c in nfkd_form if not unicodedata.combining(c)])


def load_db(dbpath):
    '''
    Load yaml database and return in a list.
    '''

    if not os.path.exists(dbpath):
        print(f'Cannot find {dbpath} directory, rerun from AppleModels directory!')
        sys.exit(1)

    db = []

    for root, _, files in os.walk(dbpath):
        for file in fnmatch.filter(files, '*.yaml'):
            path = os.path.join(root, file)
            with open(path, 'r', encoding='utf-8') as fh:
                try:
                    r = yaml.safe_load(fh)
                    if r.get('SystemProductName', None) is None:
                        print(f'WARN: Missing SystemProductName in {path}, skipping!')
                        continue
                    db.append(r)
                except yaml.YAMLError as e:
                    print(f'Failed to parse file {path} - {e}')
                    sys.exit(1)

    if len(db) == 0:
        print('Empty database!')
        sys.exit(1)

    # Sorting is required for fast lookup.
    return sorted(db, key=operator.itemgetter('SystemProductName'))


def gather_products(db, ptype='AppleModelCode', empty_valid=False, shared_valid=False, fatal=True):
    '''
    Obtain all product codes from the database
    '''
    products = []
    for info in db:
        pp = info.get(ptype, None)
        if pp is None:
            continue
        for p in pp:
            if p == '':
                if not empty_valid:
                    print(f"ERROR: {info['SystemProductName']} in contains empty {ptype}, skipping!")
                    if fatal:
                        sys.exit(1)
                continue
            if p in ['000', '0000']:
                print(f"WARN: {info['SystemProductName']} in contains zero {ptype}, skipping!")
                continue
            if p in products:
                if not shared_valid:
                    print(f"ERROR: {info['SystemProductName']} shares {ptype} {p} with other model!")
                    if fatal:
                        sys.exit(1)
                continue
            products.append(p)
    return products


def validate_products(db, dbpd):
    usedproducts = gather_products(db)

    # FIXME: Empty is not valid, but we let it be for now.
    gather_products(db, 'AppleBoardCode', True, True, False)

    knownproducts = dbpd
    for product in usedproducts:
        if knownproducts.get(product, None) is None:
            print(f'ERROR: Model {product} is used in DataBase but not present in Products!')
            sys.exit(1)
        if knownproducts[product][update_products.KEY_STATUS] != update_products.STATUS_OK:
            print(f'ERROR: Model {product} is used in DataBase but not valid in Products!')
            sys.exit(1)

    to_add = {}

    for product in knownproducts:
        if knownproducts[product][update_products.KEY_STATUS] != update_products.STATUS_OK:
            continue

        name = knownproducts[product][update_products.KEY_NAME]
        if name.find('Mac') < 0 and name.find('Xserve') < 0:
            continue
        if name.find('M1') >= 0:
            continue

        if len(product) > 3 and product not in usedproducts:
            print(f'WARN: Model {product} ({name}) is known but is not used in DataBase!')

            if to_add.get(name, None) is None:
                to_add[name] = []
            to_add[name].append(product)
            continue

    if len(to_add) > 0:
        for sysname in to_add.items():
            for info in db:
                if sysname in info['Specifications']['SystemReportName']:
                    print(f"New AppleModelCode for {info['SystemProductName']}:")
                    for model in to_add[sysname]:
                        print(f"  - \"{model}\"")


def export_db_macinfolib(db, path, year=0):
    '''
    Export yaml database to MacInfoLib format.
    TODO: use jinja2?
    '''

    with open(path, 'w', encoding='utf-8') as fh:
        print('// DO NOT EDIT! This is an autogenerated file.', file=fh)
        print('#include "MacInfoInternal.h"', file=fh)
        print('CONST MAC_INFO_INTERNAL_ENTRY gMacInfoModels[] = {', file=fh)

        for info in db:
            if max(info['AppleModelYear']) < year:
                continue

            sb_model = info.get('AppleModelId')
            sb_model = f'"{sb_model.lower()}"' if sb_model else 'NULL'

            print(
                ' {\n'
                f'''  .SystemProductName = "{info['SystemProductName']}",\n'''
                f'''  .BoardProduct = "{info['BoardProduct'][0] if isinstance(info['BoardProduct'], list) else info['BoardProduct']}",\n'''
                f'''  .BoardRevision = {f"0x{info['BoardRevision']:X}" if 'BoardRevision' in info else 'MAC_INFO_BOARD_REVISION_MISSING'},\n'''
                f'''  .SmcRevision = {{{', '.join(map(str, info.get('SmcRevision', [0x00])))}}},\n'''
                f'''  .SmcBranch = {{{', '.join(map(str, info.get('SmcBranch', [0x00])))}}},\n'''
                f'''  .SmcPlatform = {{{', '.join(map(str, info.get('SmcPlatform', [0x00])))}}},\n'''
                f'''  .BIOSVersion = "{info['BIOSVersion']}",\n'''
                f'''  .BIOSReleaseDate = "{info['BIOSReleaseDate']}",\n'''
                f'''  .SystemVersion = "{info['SystemVersion']}",\n'''
                f'''  .SystemSKUNumber = "{info['SystemSKUNumber']}",\n'''
                f'''  .SystemFamily = "{info['SystemFamily']}",\n'''
                f'''  .BoardVersion = "{info['BoardVersion']}",\n'''
                f'''  .BoardAssetTag = "{info['BoardAssetTag']}",\n'''
                f'''  .BoardLocationInChassis = "{info['BoardLocationInChassis']}",\n'''
                f'''  .SmcGeneration = 0x{info['SmcGeneration']:X},\n'''
                f'''  .BoardType = 0x{info['BoardType']:X},\n'''
                f'''  .ChassisType = 0x{info['ChassisType']:X},\n'''
                f'''  .MemoryFormFactor = 0x{info['MemoryFormFactor']:X},\n'''
                f'''  .PlatformFeature = {f"0x{info['PlatformFeature']:X}" if 'PlatformFeature' in info else 'MAC_INFO_PLATFORM_FEATURE_MISSING'},\n'''
                f'''  .ChassisAssetTag = "{info['ChassisAssetTag']}",\n'''
                f'''  .FirmwareFeatures = 0x{info.get('ExtendedFirmwareFeatures', info.get('FirmwareFeatures', 0)):X}ULL,\n'''
                f'''  .FirmwareFeaturesMask = 0x{info.get('ExtendedFirmwareFeaturesMask', info.get('FirmwareFeaturesMask', 0)):X}ULL,\n'''
                f'  .SecureBootModel = {sb_model},\n'
                f' }},', file=fh)

        print('};', file=fh)

        print('CONST UINTN gMacInfoModelCount = ARRAY_SIZE (gMacInfoModels);', file=fh)
        print('CONST UINTN gMacInfoDefaultModel = 0;', file=fh)


def export_db_macserial(db, dbpd, path, year=0):
    '''
    Export yaml database to macserial format.
    TODO: use jinja2?
    '''

    with open(path, 'w', encoding='utf-8') as fh:
        print('#ifndef GENSERIAL_MODELINFO_AUTOGEN_H', file=fh)
        print('#define GENSERIAL_MODELINFO_AUTOGEN_H\n', file=fh)
        print('// DO NOT EDIT! This is an autogenerated file.\n', file=fh)
        print('#include "macserial.h"\n', file=fh)

        print('typedef enum {', file=fh)

        for info in db:
            print(f"  {info['SystemProductName'].replace(',', '_')}, // {info['Specifications']['CPU'][0]}", file=fh)

        print('} AppleModel;\n', file=fh)
        print(f'#define APPLE_MODEL_MAX {len(db)}\n', file=fh)

        print('static PLATFORMDATA ApplePlatformData[] = {', file=fh)
        for info in db:
            print(f'''  {{ "{info['SystemProductName']}", "{info['SystemSerialNumber']}" }},''', file=fh)

        print('};\n', file=fh)

        print(f"#define APPLE_MODEL_CODE_MAX {max(len(info['AppleModelCode']) for info in db)}", file=fh)
        print('static const char *AppleModelCode[][APPLE_MODEL_CODE_MAX] = {', file=fh)

        for info in db:
            print(f"""  /* {info['SystemProductName']:14} */ {{"{'", "'.join(info['AppleModelCode'])}"}},""", file=fh)

        print('};\n', file=fh)

        print(f"#define APPLE_BOARD_CODE_MAX {max(len(info['AppleBoardCode']) for info in db)}", file=fh)
        print('static const char *AppleBoardCode[][APPLE_BOARD_CODE_MAX] = {', file=fh)

        for info in db:
            print(f"""  /* {info['SystemProductName']:14} */ {{"{'", "'.join(info['AppleBoardCode'])}"}},""", file=fh)

        print('};\n', file=fh)

        print(f"#define APPLE_MODEL_YEAR_MAX {max(len(info['AppleModelYear']) for info in db)}", file=fh)
        print('static uint32_t AppleModelYear[][APPLE_MODEL_YEAR_MAX] = {', file=fh)
        for info in db:
            print(f"  /* {info['SystemProductName']:14} */ {{{', '.join(str(year) for year in info['AppleModelYear'])}}},", file=fh)

        print('};\n', file=fh)

        print('static uint32_t ApplePreferredModelYear[] = {', file=fh)
        for info in db:
            print(f"  /* {info['SystemProductName']:14} */ {info.get('MacserialModelYear', 0)},", file=fh)

        print('};\n', file=fh)

        print('static APPLE_MODEL_DESC AppleModelDesc[] = {', file=fh)

        models = sorted(dbpd.keys())
        models.sort(key=len)

        for model in models:
            if dbpd[model][update_products.KEY_STATUS] == update_products.STATUS_OK:
                print(f""" {{"{model}", "{remove_accents(dbpd[model][update_products.KEY_NAME])}"}},""", file=fh)

        print('};\n', file=fh)

        print('#endif // GENSERIAL_MODELINFO_AUTOGEN_H', file=fh)


def export_mlb_boards(db, boards):

    mlb = {}
    for info in db:
        if len(info['SystemSerialNumber']) == 12:
            models = [info['BoardProduct']] if not isinstance(info['BoardProduct'], list) else info['BoardProduct']

            for model in models:
                mlb[model] = 'latest' if not info['MaximumOSVersion'] else info['MaximumOSVersion']

    with open(boards, 'w', encoding='utf-8') as fh:
        json.dump(mlb, fh, indent=1)


if __name__ == '__main__':

    db = load_db('DataBase')
    dbpd = update_products.load_products()
    # Run test phase to validate the library
    validate_products(db, dbpd)
    export_db_macinfolib(db, os.devnull)
    export_db_macserial(db, dbpd, os.devnull)
    # Export new models
    export_db_macinfolib(db, '../Library/OcMacInfoLib/AutoGenerated.c')
    export_db_macserial(db, dbpd, '../Utilities/macserial/modelinfo_autogen.h')
    # Export MLB models
    export_mlb_boards(db, '../Utilities/macrecovery/boards.json')

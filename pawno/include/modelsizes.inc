/*
	Thanks Y-Less for releasing the old one (modelsizes).
	Thanks Pottus, Chris420, and Slice for ColAndreas (making this possible).
	Thanks Abagail and Crayder for creating modelsizes.

	SQL Version of modelsizes by: algorhitmically
	
	Function "GetObjectModelName" add by: ForT
*/

#if defined _objectinfo_included
	#endinput
#endif
#define _objectinfo_included

#include <YSI_Coding\y_hooks>

static DB:ModelSizes;

hook OnScriptInit()
{
	ms_LoadDB();

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnScriptExit()
{
	if(db_get_mem_handle(ModelSizes) != 0)
	{
		db_close(ModelSizes);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

stock ms_LoadDB()
{
	if(db_get_mem_handle(ModelSizes) != 0)
	{
		return 1;
	}
	if(!fexist("modelsizes.db"))
	{
		print("ERROR: (modelsizes-SQL): Cannot find \"modelsizes.db\".");
		return 0;
	}
	if((ModelSizes = db_open("modelsizes.db")) == DB:0)
	{
		print("ERROR: (modelsizes-SQL): Failed to load database \"modelsizes.db\".");
		return 0;
	}
	else
	{
		print("(modelsizes-SQL): Database loaded successfully.");
	}

	return 1;
}

stock GetColCount()
{
	new dOut = 0, DBResult:query;

    query = db_query(ModelSizes, "SELECT COUNT(ColRadius) AS ColCount FROM modelsizes");
    if(db_num_rows(query) > 0)
	{
        dOut = db_get_field_assoc_int(query, "ColCount");
        db_free_result(query);
		return dOut;
    }
    return 0;
}

stock Float:GetColSphereRadius(objectmodel)
{
	new Query[60], Float:dOut = 0.0, DBResult:query;

    format(Query, sizeof Query, "SELECT ColRadius FROM modelsizes WHERE modelid = %d", objectmodel);
    query = db_query(ModelSizes, Query);
	if(db_num_rows(query) > 0)
	{
 		dOut = db_get_field_assoc_float(query, "ColRadius");
        db_free_result(query);
		return dOut;
	}
	return 0.0;
}

stock GetColSphereOffset(objectmodel, &Float:x, &Float:y, &Float:z)
{
	new Query[86], DBResult:query;

    format(Query, sizeof Query, "SELECT ColOffsetX, ColOffsetY, ColOffsetZ FROM modelsizes WHERE modelid = %d", objectmodel);
    query = db_query(ModelSizes, Query);
   	if(db_num_rows(query) > 0)
	{
		x = db_get_field_assoc_float(query, "ColOffsetX");
		y = db_get_field_assoc_float(query, "ColOffsetY");
		z = db_get_field_assoc_float(query, "ColOffsetZ");
		db_free_result(query);
		return 1;
	}
	else
	{
		x = y = z = 0.0;
	}
	return 0;
}

stock GetModelBoundingBox(objectmodel, &Float:MinX, &Float:MinY, &Float:MinZ, &Float:MaxX, &Float:MaxY, &Float:MaxZ)
{
	new Query[86], DBResult:query;

    format(Query, sizeof Query, "SELECT MinX, MinY, MinZ, MaxX, MaxY, MaxZ FROM modelsizes WHERE modelid = %d", objectmodel);
    query = db_query(ModelSizes, Query);
   	if(db_num_rows(query) > 0)
	{
		MinX = db_get_field_assoc_float(query, "MinX");
		MinY = db_get_field_assoc_float(query, "MinY");
		MinZ = db_get_field_assoc_float(query, "MinZ");
		MaxX = db_get_field_assoc_float(query, "MaxX");
		MaxY = db_get_field_assoc_float(query, "MaxY");
		MaxZ = db_get_field_assoc_float(query, "MaxZ");
		db_free_result(query);
		return 1;
	}
	else
	{
		MinX = MinY = MinZ = MaxX = MaxY = MaxZ = 0.0;
	}
	return 0;
}

stock GetModelColDimensions(objectmodel, &Float:l, &Float:w, &Float:h)
{
	new Query[86], DBResult:query;

    format(Query, sizeof Query, "SELECT MinX, MinY, MinZ, MaxX, MaxY, MaxZ FROM modelsizes WHERE modelid = %d", objectmodel);
    query = db_query(ModelSizes, Query);
   	if(db_num_rows(query) > 0)
	{
		l = floatabs(db_get_field_assoc_float(query, "MaxX") - db_get_field_assoc_float(query, "MinX"));
		w = floatabs(db_get_field_assoc_float(query, "MaxY") - db_get_field_assoc_float(query, "MinY"));
		h = floatabs(db_get_field_assoc_float(query, "MaxZ") - db_get_field_assoc_float(query, "MinZ"));
		db_free_result(query);
		return 1;
	}
	else
	{
		l = w = h = 0.0;
	}
	return 0;
}

stock GetObjectModelName(objectmodel, dest[], length = sizeof(dest))
{
	new Query[60], DBResult:query;

    format(Query, sizeof Query, "SELECT name FROM modelsizes WHERE modelid = %d", objectmodel);
    query = db_query(ModelSizes, Query);
   	if(db_num_rows(query) > 0)
	{
        db_get_field_assoc(query, "name", dest, length);
        db_free_result(query);
		return 1;
	}
	else
	{
	    dest[0] = EOS;
	}
	return 0;
}

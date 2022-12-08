--complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION kalyna" to load this file. \quit

CREATE OR REPLACE FUNCTION kalyna_encrypt(text, text) RETURNS bytea
AS '$libdir/kalyna'
LANGUAGE C IMMUTABLE STRICT;
